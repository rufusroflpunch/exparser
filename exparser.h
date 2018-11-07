#include <memory>
#include <string>
#include <locale>
#include <vector>
#include <sstream>

using namespace std;

class Node;

using NodePtr = unique_ptr<Node>;
using StringIterType = decltype(string{}.begin());
using TokenIterType = decltype(vector<string>{}.begin());

class Node
{
  public:
  virtual ~Node() {}
  virtual string pprint() { throw "Invalid"; };
  virtual double eval() { throw "Invalid"; };
};

class NumNode : public Node
{
  public:
  double val;
  NumNode(double val) : val(val) {}
  string pprint() override { return to_string(val); }
  double eval() override { return val; }
};

class BinOpNode : public Node
{
  public:
  NodePtr lhs, rhs;
  string op;
  BinOpNode(string op, NodePtr lhs, NodePtr rhs) : op(op), lhs(move(lhs)), rhs(move(rhs)) {}
  string pprint() override {
    stringstream s;
    s << "(" << lhs->pprint() << " " << op << " " << rhs->pprint() << ")";
    return s.str();
  }
  double eval() override {
    double l = lhs->eval(), r = rhs->eval();
    if (op == "+") return l + r;
    if (op == "-") return l - r;
    if (op == "*") return l * r;
    if (op == "/") return l / r;
    throw "Noop!";
  }
};

string get_token(StringIterType& begin, const StringIterType& end)
{
  while (begin < end)
  {
    while (isspace(*begin)) begin++;
    if (isdigit(*begin) || *begin == '.') {
      string num{};
      while (isdigit(*begin) || *begin == '.') num += *(begin++);
      return num;
    } else {
      return string{*(begin++)};
    }
  }
  return string{};
}

NodePtr parse_num(TokenIterType& begin, const TokenIterType& end)
{
  if (begin >= end) return nullptr;
  try {
    double num = stod(*begin);
    begin++;
    return make_unique<NumNode>(num);
  } catch(std::exception) {
    return nullptr;
  }
}

NodePtr parse_mul(TokenIterType& begin, const TokenIterType& end)
{
  auto orig = begin;
  try {
    if (begin >= end) throw 0;
    NodePtr lhs, rhs;
    string op;
    lhs = parse_num(begin, end);
    if (!lhs) throw 0;
    op = *begin;
    if (op != "*" && op != "/") return lhs;
    if (++begin >= end) throw 0;
    rhs = parse_mul(begin, end);
    if (!rhs) throw 0;
    return make_unique<BinOpNode>(op, move(lhs), move(rhs));
  } catch(int) {
    begin = orig;
    return nullptr;
  }
}

NodePtr parse_add(TokenIterType& begin, const TokenIterType& end)
{
  auto orig = begin;
  try {
    if (begin >= end) throw 0;
    NodePtr lhs, rhs;
    string op;
    lhs = parse_mul(begin, end);
    if (!lhs) throw 0;
    op = *begin;
    if (op != "+" && op != "-") return lhs;
    if (++begin >= end) throw 0;
    rhs = parse_add(begin, end);
    if (!rhs) throw 0;
    begin++;
    return make_unique<BinOpNode>(op, move(lhs), move(rhs));
  } catch(int) {
    begin = orig;
    return nullptr;
  }
}
