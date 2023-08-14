#include <exception>
#include <string>

namespace {
    class EZRubyException : public std::exception
    {
    public:
        EZRubyException() : m_message("EzRuby exception thrown.") {}
        EZRubyException(const std::string& message) : m_message(message) {}

        virtual const char* what() const noexcept override
        {
            return m_message.c_str();
        }

    private:
        std::string m_message;
    };
}