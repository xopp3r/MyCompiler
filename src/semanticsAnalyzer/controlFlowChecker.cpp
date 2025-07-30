#include "controlFlowChecker.hpp"
#include "../common/nodes.hpp"
#include "../common/types.hpp"






// template<typename T>
// concept Iterable = requires(T t) {
//     { begin(t) } -> std::input_or_output_iterator;
//     { end(t) } -> std::sentinel_for<decltype(begin(t))>;
// };




// template <Iterable Iter1, Iterable Iter2>
// class JoinedIterator {
// public:
//     JoinedIterator& operator++(){
//         if (current1 =! end1) current1++;
//         else current2++;

//         return *this;
//     }

//     bool operator!=(const JoinedIterator& other) const {
//         return current1 != other.current1 || current2 != other.current2;
//     }

//     auto operator*() const {
//         return current1 != end1 ? *current1 : *current2;
//     }

// private:
//     Iter1 current1;
//     Iter1 end1;
//     Iter2 current2;
// };





// template<typename Container1, typename Container2>
// auto join(Container1& c1, Container2& c2) {
//     return std::make_pair(
//         JoinedIterator(c1.begin(), c1.end(), c2.begin()),
//         JoinedIterator(c1.end(), c1.end(), c2.end())
//     );
// }





template <TokenType ...types>
static bool isOneOf(auto& stmnt) {
    return ((stmnt->token.type == types) || ...);
}





void ControlFlowChecker::visit(FunctionDefinition& node) {
    if (getType(node.token.lexeme) == TYPE_VOID) return;



}

