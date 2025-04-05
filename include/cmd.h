#ifndef INC__COMMAND__H
#define INC__COMMAND__H

#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include <objh.h>
#include <enum.h>

class Module : public ReportInterface
{
    using Callback = std::function<void(Module& mod)>;
    using DefaultFunc = std::function<void(const std::string&)>;

    enum Config
    {
        DEFAULT = 0,
        NO_REPORT = 1,
    };

    struct OptionInfo
    {
        std::string name;
        std::string short_name;
        std::string description;
        Callback callback;
    };

    class Util
    {
    public:

        constexpr
        Util(Module& target)
        : _M_mod(target) { }

        template <typename Type>
        constexpr
        void dumpNext(Type* dst)
        {
            _M_mod._M_buff.clear();
            _M_mod._M_buff << next();
            _M_mod._M_buff >> *dst;
        }

        constexpr
        const std::string& peek() const
        {
            return _M_mod._M_peek();
        }

        constexpr
        const std::string& next()
        {
            return _M_mod._M_next();
        }

        constexpr
        bool expect(const std::string& value)
        {
            return _M_mod._M_expect(value);
        }

        constexpr
        bool good() const
        {
            return _M_mod._M_good();
        }

        void report(const std::string& msg, bool throw_msg = false)
        {
            return _M_mod._M_report(msg, throw_msg);
        }

        bool nextIsArg() const
        {
            return good() && !_M_mod._M_is_option(peek());
        }

        bool nextIsOpt() const
        {
            return !nextIsArg();
        }

    private:

        Module& _M_mod;
    };

public:

    Module(const std::string& name)
    : _M_name(name)
    {
        _M_regist_option("help", "get help and usage", _S_help);
    }

    Module(const Module&) = delete;
    Module(Module&&) = delete;

    /**
     * avaliable only during parsing
     */
    constexpr
    Util utils()
    {
        if (_M_current)
            return Util(*this);
        else
            return Util(*((Module*)(0)));
    }

    void parse(int argc, char const *argv[])
    {
        _M_current = 0;
        _M_args = std::vector<std::string>(argv+1, argv+argc);

        for (const std::string* arg; _M_good();)
        {
            arg = &_M_next();
            Callback call = _M_get_callback(*arg);
            if (call)
            {
                call(*this);
            }
            else
            {
                if (_M_default)
                    _M_default(*arg);
                else
                    _M_report("[UNKNOWN] '" + *arg + "'");
            }
        }
    }

    void addOption(const std::string& full_name, Callback operation)
    {
        _M_regist_option(full_name, "", operation);
    }

    void addOption(const std::string& full_name, const std::string& description, Callback operation)
    {
        _M_regist_option(full_name, description, operation);
    }

    void setDefault(DefaultFunc func)
    {
        this->_M_default = func;
    }

    constexpr
    void report(bool value) noexcept
    {
        if (value)
            _M_config |= NO_REPORT;
        else
            _M_config &= ~NO_REPORT;
    }

private:

    static
    void _S_help(Module& mod)
    {
        std::cout << mod._M_name << " Usage: [-OPTIONS] [ARGS...]" << '\n';
        for (auto info : mod._M_options)
        {
            std::cout << "  " << info.short_name << ' ' << info.name << '\n';
            if (!info.description.empty())
            {
                std::cout.put('\t');
                std::cout.put('\t');
                for (const char& ch : info.description)
                {
                    std::cout.put(ch);
                    if (ch == '\n')
                    {
                        std::cout.put('\t');
                        std::cout.put('\t');
                    }
                }
                std::cout.put('\n');
            }
        }
    }

    void _M_report(const std::string& message, bool throw_msg = false) const
    {
        _S_do_report(_S_make_message(this->_M_name, message), throw_msg);
    }

    void _M_regist_option(const std::string& opt, const std::string& description, Callback call_back)
    {
        size_t index = _M_options.size();
        _M_options.emplace_back("--"+opt, std::string{'-', opt[0]}, description, call_back);
        _M_option_indice.emplace(_M_options.back().name.c_str(), index);
        _M_option_indice.emplace(_M_options.back().short_name.c_str(), index);
    }

    constexpr
    const std::string& _M_peek() const
    {
        return _M_args.at(_M_current);
    }

    constexpr
    const std::string& _M_next()
    {
        const std::string& str = _M_peek();
        ++_M_current;
        return str;
    }

    constexpr
    bool _M_expect(const std::string& value)
    {
        if (_M_peek() == value)
        {
            _M_next();
            return true;
        }
        return false;
    }

    constexpr
    bool _M_good() const
    {
        return _M_current < _M_args.size();
    }

    Callback _M_get_callback(const std::string& opt) const
    {
        auto it = _M_option_indice.find(opt);
        if (it != _M_option_indice.end())
        {
            return _M_options[it->second].callback;
        }
        return nullptr;
    }

    bool _M_is_option(const std::string& str) const
    {
        return _M_option_indice.find(str) != _M_option_indice.end();
    }

private:

    Enum<Config> _M_config;
    const std::string _M_name;

    std::map<std::string, size_t> _M_option_indice;
    std::vector<OptionInfo> _M_options;

    DefaultFunc _M_default;

    std::vector<std::string> _M_args;
    std::size_t _M_current{0};
    std::stringstream _M_buff;
};

#endif