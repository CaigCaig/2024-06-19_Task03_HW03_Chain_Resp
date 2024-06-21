#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef enum
{
    WARNING,
    ERROR,
    FATAL_ERROR,
    UNKNOWN
} Type;

class LogMessage {
public:
    Type type() const;
    const std::string& message() const;
};

class BaseHandler
{
public:
    virtual void setNextHandler(BaseHandler* handler) = 0;
    virtual void handlerRequest(const LogMessage& message) = 0;
    virtual void setPath(const string& path_) = 0;
};

// Обработчик предупреждения должен напечатать сообщение в консоль.
class WarningHandler : public BaseHandler
{
private:
    BaseHandler* nextHandler = nullptr;
public:
    void setNextHandler(BaseHandler* handler) override
    {
        nextHandler = handler;
    }
    void handlerRequest(const LogMessage& message) override
    {
        if (message.type == WARNING)
        {
            cout << message.message << endl;
        }
        else if (nextHandler != nullptr)
        {
            nextHandler->handlerRequest(message);
        }
        else
        {
            cout << "Warning handler: unknown type of message" << endl;
        }
    }
    virtual void setPath(const string& path_) {}
};

// Обработчик обычной ошибки должен записать её в файл по указанному пути.
class ErrorHandler : public BaseHandler
{
private:
    BaseHandler* nextHandler = nullptr;
    string path;
public:
    void setNextHandler(BaseHandler* handler) override
    {
        nextHandler = handler;
    }
    void handlerRequest(const LogMessage& message) override
    {
        if (type == ERROR)
        {
            std::ofstream fout{ this->path, ios::app };      // открываем файл для записи
            if (fout.is_open())
            {
                fout << "Fatal Error: " << request << endl;
            }
            fout.close();
            //cout << "Authentification using login and password" << endl;
        }
        else if (nextHandler != nullptr)
        {
            nextHandler->handlerRequest(message);
        }
        else
        {
            cout << "Error handler: unknown type of message" << endl;
        }
    }
    void setPath(const string& p)
    {
        path = p;
    }
};

//Обработчик фатальной ошибки должен выбросить исключение с текстом сообщения, потому что предполагается, что программа должна прекратить выполнение после возникновения фатальной ошибки.
class FatalErrorHandler : public BaseHandler
{
private:
    BaseHandler* nextHandler = nullptr;
public:
    void setNextHandler(BaseHandler* handler) override
    {
        nextHandler = handler;
    }
    void handlerRequest(const LogMessage& message) override
    {
        try
        {
            if (type == FATAL_ERROR)
            {
                throw exception("FATAL ERROR!");             
            }
            else if (nextHandler != nullptr)
            {
                nextHandler->handlerRequest(message);
            }
            else
            {
                cout << "Fatal error handler: unknown type of message" << endl;
            }
        }
        catch (const std::exception& ex)
        {
            cout << request << endl;
        }
    }
    virtual void setPath(const string& path_) {}
};

// Обработчик неизвестного сообщения должен выбросить исключение с текстом о необработанном сообщении.
class UnknownHandler : public BaseHandler
{
private:
    BaseHandler* nextHandler = nullptr;
public:
    void setNextHandler(BaseHandler* handler) override
    {
        nextHandler = handler;
    }
    void handlerRequest(const LogMessage& message) override
    {
        try
        {
            if (type == UNKNOWN)
            {
                throw exception("Unknown message!");
                cout << "Authentification using login and password" << endl;
            }
            else if (nextHandler != nullptr)
            {
                nextHandler->handlerRequest(message);
            }
            else
            {
                cout << "Unknown handler: unknown type of message" << endl;
            }
        }
        catch (const std::exception& ex)
        {
            cout << "Unknown handler: The message has not been processed" << endl;
        }
    }
    virtual void setPath(const string& path_) {}
};

int main()
{
    LogMessage message;
    BaseHandler* warning = new WarningHandler();
    BaseHandler* error = new ErrorHandler();
    BaseHandler* fatal_error = new FatalErrorHandler();
    BaseHandler* unknown = new UnknownHandler();

    error->setPath("errors.log");

    fatal_error->setNextHandler(error);
    error->setNextHandler(warning);
    warning->setNextHandler(unknown);

    message.message = "WARNING1!"
    fatal_error->handlerRequest(message);
    fatal_error->handlerRequest(WARNING, "WARNING1!");
    fatal_error->handlerRequest(ERROR, "ERROR1!");
    fatal_error->handlerRequest(FATAL_ERROR, "FATAL_ERROR1!");
    fatal_error->handlerRequest(UNKNOWN, "UNKNOWN1!");

    delete fatal_error;
    delete error;
    delete warning;
    delete unknown;

    system("pause");
}

/*
Задание 3. Паттерн «Цепочка ответственности»
Продолжаем тему с реализацией системы логирования в соответствии с разными паттернами проектирования.

В задании нужно реализовать сообщение о логировании с типом сообщения и самим сообщением:

class LogMessage {
public:
    Type type() const;
    const std::string& message() const;
};
Всего должно быть четыре типа сообщений:

предупреждение,
ошибка,
фатальная ошибка,
неизвестное сообщение.
Далее нужно добавить обработчиков для разных типов, которые будут обрабатывать сообщения по приоритету:

фатальная ошибка,
ошибка,
предупреждение,
неизвестное сообщение.
Каждый обработчик умеет:

обрабатывать сообщение только определённого типа,
передавать сообщение следующему обработчику.
Вот что обработчик должен уметь в зависимости от типа сообщения.

Обработчик фатальной ошибки должен выбросить исключение с текстом сообщения, потому что предполагается, что программа должна прекратить выполнение после возникновения фатальной ошибки.
Обработчик обычной ошибки должен записать её в файл по указанному пути.
Обработчик предупреждения должен напечатать сообщение в консоль.
Обработчик неизвестного сообщения должен выбросить исключение с текстом о необработанном сообщении.
*/