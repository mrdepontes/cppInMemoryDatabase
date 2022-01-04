#include <iostream>
#include <string>
#include <map>
#include <boost/json.hpp>
#include <boost/filesystem/string_file.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include "lib/IN_PLACE_DATABASE.h"

template <typename K, typename T>
struct Record : std::map<K, T>
{
};

struct Pokemon
{
    std::string id = "unknown";
    boost::int64_t attack = 0;
    boost::int64_t defense = 0;
    boost::int64_t health = 0;

    Pokemon() = default;

    Pokemon(std::string id, std::uint64_t attack, std::uint64_t defense, std::uint64_t health)
        : id(id), attack(attack), defense(defense), health(health) {}
};
Pokemon tag_invoke(boost::json::value_to_tag<Pokemon>, boost::json::value const &v)
{
    Pokemon pokemon(
        (std::string)v.at("id").as_string(),
        v.at("attack").as_int64(),
        v.at("defense").as_int64(),
        v.at("health").as_int64());
    return pokemon;
}

struct BaseRecord
{
    std::string id;
};

template <typename T>
class Database : BaseRecord
{
    void set(T newValue);
    T get(std::string id);
};

template <typename T>
class InMemoryDataBase : Database<T>
{
private:
    Record<std::string, T> db = {};

public:
    InMemoryDataBase(){};
    ~InMemoryDataBase(){};
    void set(T newValue);
    T get(std::string id);
    void getAll(std::function<void(T)>);
    T selectBest(std::function<boost::int64_t(T)>);
};

template <typename T>
void InMemoryDataBase<T>::set(T newValue)
{
    db[newValue.id] = newValue;
    return;
};

template <typename T>
T InMemoryDataBase<T>::get(std::string id)
{
    return db[id];
}

// Visitor Pattern
template <typename T>
void InMemoryDataBase<T>::getAll(std::function<void(T)> func)
{
    for (auto &pokemon : db)
    {
        func(pokemon.second);
    }
    return;
}
// template <typename T>
// struct found
// {
//     void operator()(T p) { pokemon = pokemon; }
//     boost::int64_t max = 0;
//     T pokemon = {};
// };
// Stratagy Pattern
template <typename T>
T InMemoryDataBase<T>::selectBest(std::function<boost::int64_t(T pokemon)> scoreStratagy)
{

    struct found
    {
        boost::int64_t max = 0;
        T pokemon = {};
    };

    // found<T> item;
    found item;
    std::for_each(db.begin(), db.end(),
                //   [scoreStratagy, &item](auto pokemon) mutable
                  [&](auto pokemon) mutable
                  {
                      boost::int64_t score = scoreStratagy(pokemon.second);
                      if (score >= item.max)
                      {
                          item.max = score;
                          item.pokemon = pokemon.second;
                          //   item(pokemon.second);
                      }
                      return item;
                  });

    return item.pokemon;
}

template <typename T>
InMemoryDataBase<T> createDatabase()
{
    return InMemoryDataBase<T>();
}

auto PokemonDB = createDatabase<Pokemon>();

class PokemonDBAdator : public RecordHandler<Pokemon>
{
public:
    void addRecord(Pokemon record)
    {
        PokemonDB.set(record);
        return;
    }
};

int main(int argc, const char **argv)
{
    Pokemon squirtl("Squirtl", 25, 60, 100);
    PokemonDB.set(squirtl);

    Pokemon warTortle("WarTortle", 75, 60, 150);
    PokemonDB.set(warTortle);

    std::cout << PokemonDB.get("Squirtl").id << " Attack:" << PokemonDB.get("Squirtl").attack << std::endl;

    loader<Pokemon>("../../data.json", std::make_shared<PokemonDBAdator>());
    std::cout << PokemonDB.get("Onix").id << " Attack:" << PokemonDB.get("Onix").attack << std::endl;

    PokemonDB.getAll([](Pokemon pokemon) -> void
                     {
                         std::cout << "Pokemon " << pokemon.id << " with ATK " << pokemon.attack << '\n';
                         return;
                     });

    auto bestDefensive = PokemonDB.selectBest(
        [](Pokemon pokemon) -> boost::int64_t
        {
            return pokemon.defense;
        });
    std::cout << "Best Defense = " << bestDefensive.id << std::endl;

    auto bestAttack = PokemonDB.selectBest(
        [](Pokemon pokemon) -> boost::int64_t
        {
            return pokemon.attack;
        });
    std::cout << "Best Attack = " << bestAttack.id << std::endl;
    return 0;
}
