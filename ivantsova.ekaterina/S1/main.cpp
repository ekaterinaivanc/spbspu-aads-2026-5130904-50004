#include <iostream>
#include <string>
#include <utility>
#include <list.hpp>

int main()
{
  try
  {
    ivantsova::List< std::pair< std::string, ivantsova::List< unsigned long long > > > sequences;
    std::string title;

    while (std::cin >> title)
    {
      ivantsova::List< unsigned long long > numbers;
      unsigned long long num;
      while (std::cin >> num)
      {
        numbers.push_back(num);
      }
      std::cin.clear();
      sequences.push_back({title, std::move(numbers)});
    }

    if (sequences.empty())
    {
      std::cout << "0" << "\n";
      return 0;
    }
    auto it = sequences.cbegin();
    std::cout << it->first;
    ++it;
    for (; it != sequences.cend(); ++it)
    {
      std::cout << ' ' << it->first;
    }
    std::cout << "\n";
    size_t maxLen = 0;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
    {
      if (it->second.size() > maxLen)
      {
        maxLen = it->second.size();
      }
    }
    ivantsova::List< ivantsova::List< unsigned long long > > transposed;
    for (size_t i = 0; i < maxLen; ++i)
    {
      ivantsova::List< unsigned long long > newList;
      for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
      {
        if (i < it->second.size())
        {
          auto elemIt = it->second.cbegin();
          for (size_t j = 0; j < i; ++j)
          {
            ++elemIt;
          }
          newList.push_back(*elemIt);
        }
      }
      transposed.push_back(std::move(newList));
    }

    for (auto it = transposed.cbegin(); it != transposed.cend(); ++it)
    {
      if (it->empty())
      {
        std::cout << "\n";
        continue;
      }

      bool firstInRow = true;
      for (auto elemIt = it->cbegin(); elemIt != it->cend(); ++elemIt)
      {
        if (!firstInRow)
        {
          std::cout << ' ';
        }
        std::cout << *elemIt;
        firstInRow = false;
      }
      std::cout << "\n";
    }
    if (maxLen == 0)
    {
      std::cout << "0\n";
      return 0;
    }
    ivantsova::List< unsigned long long > sums;
    for (auto it = transposed.cbegin(); it != transposed.cend(); ++it)
    {
      unsigned long long total = 0;
      for (auto elemIt = it->cbegin(); elemIt != it->cend(); ++elemIt)
      {
        ivantsova::sum(total, *elemIt);
      }
      sums.push_back(total);
    }
    if (!sums.empty())
    {
      auto it = sums.cbegin();
      std::cout << *it;
      ++it;
      for (; it != sums.cend(); ++it)
      {
        std::cout << ' ' << *it;
      }
      std::cout << "\n";
    }
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
