#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;
template <class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag,
                         tree_order_statistics_node_update>;

template <class key, class value, class cmp = std::less<key> >
using ordered_map
    = tree<key, value, cmp, rb_tree_tag, tree_order_statistics_node_update>;

template <class T> ostream &operator<< (ostream &os, const vector<T> &V);

template <class T> ostream &operator<< (ostream &os, const set<T> &S);

template <class T>
ostream &operator<< (ostream &os, const unordered_set<T> &S);

template <class T> ostream &operator<< (ostream &os, const multiset<T> &S);

template <class T> ostream &operator<< (ostream &os, const ordered_set<T> &S);

template <class L, class R>
ostream &operator<< (ostream &os, const pair<L, R> &P);

template <class L, class R>
ostream &operator<< (ostream &os, const map<L, R> &M);

template <class L, class R, class chash = std::hash<R> >
ostream &operator<< (ostream &os, const unordered_map<L, R, chash> &M);

template <class L, class R, class chash = std::hash<R> >
ostream &operator<< (ostream &os, const gp_hash_table<L, R, chash> &M);

ostream &
operator<< (ostream &os, const vector<uint8_t> &V)
{
  os << "[ ";
  for (auto v : V)
    os << "0x" << std::hex << int (v) << " ";
  return os << "]";
}
template <class T>
ostream &
operator<< (ostream &os, const vector<T> &V)
{
  os << "[ ";
  for (auto v : V)
    os << v << " ";
  return os << "]";
}

template <class T>
ostream &
operator<< (ostream &os, const set<T> &S)
{
  os << "{ ";
  for (auto s : S)
    os << s << " ";
  return os << "}";
}
template <class T>
ostream &
operator<< (ostream &os, const unordered_set<T> &S)
{
  os << "{ ";
  for (auto s : S)
    os << s << " ";
  return os << "}";
}
template <class T>
ostream &
operator<< (ostream &os, const multiset<T> &S)
{
  os << "{ ";
  for (auto s : S)
    os << s << " ";
  return os << "}";
}
template <class T>
ostream &
operator<< (ostream &os, const ordered_set<T> &S)
{
  os << "{ ";
  for (auto s : S)
    os << s << " ";
  return os << "}";
}
template <class L, class R>
ostream &
operator<< (ostream &os, const pair<L, R> &P)
{
  return os << "(" << P.first << "," << P.second << ")";
}
template <class L, class R>
ostream &
operator<< (ostream &os, const map<L, R> &M)
{
  os << "{ ";
  for (auto m : M)
    os << "(" << m.first << ":" << m.second << ") ";
  return os << "}";
}
template <class L, class R, class chash = std::hash<R> >
ostream &
operator<< (ostream &os, const unordered_map<L, R, chash> &M)
{
  os << "{ ";
  for (auto m : M)
    os << "(" << m.first << ":" << m.second << ") ";
  return os << "}";
}
template <class L, class R, class chash = std::hash<R> >
ostream &
operator<< (ostream &os, const gp_hash_table<L, R, chash> &M)
{
  os << "{ ";
  for (auto m : M)
    os << "(" << m.first << ":" << m.second << ") ";
  return os << "}";
}
