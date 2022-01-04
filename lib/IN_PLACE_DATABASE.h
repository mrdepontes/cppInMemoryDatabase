#ifndef IN_PLACE_DATABASE
#define IN_PLACE_DATABASE

#include <iostream>
#include <string>

template <typename T>
class RecordHandler
{
public:
    virtual void addRecord(T record){};
};

template <typename T>
void loader(std::string filename, std::shared_ptr<RecordHandler<T>> recordHandler)
{
    boost::json::error_code ec;
    std::string result;
    boost::filesystem::load_string_file(filename, result);
    boost::json::value data = boost::json::parse(result, ec);
    if (data.is_array())
    {
        for (auto &record : data.get_array())
        {
            if (record.is_object())
            {
                T pokemon = boost::json::value_to<T>(record);
                recordHandler->addRecord(pokemon);
            }
        }
    }
}

#endif // IN_PLACE_DATABASE