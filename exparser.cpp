#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <memory>

#include "exparser.h"

using namespace std;

int main()
{
  string exp = "30 * 10 + 5 / 12 + 14";
  auto exp_begin = begin(exp), exp_end = end(exp);
  auto token = get_token(exp_begin, exp_end);
  vector<string> tokens;
  while (token != "")
  {
    tokens.push_back(token);
    token = get_token(exp_begin, exp_end);
  }
  
  auto tokens_begin = begin(tokens), tokens_end = end(tokens);
  while (tokens_begin < tokens_end) {
    auto num = parse_add(tokens_begin, tokens_end);
    cout << num->pprint() << " == " << num->eval() << '\n';
  }
}
