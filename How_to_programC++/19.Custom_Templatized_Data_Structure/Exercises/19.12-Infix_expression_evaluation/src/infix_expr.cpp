#include <algorithm>
#include <cctype>
#include "infix_expr.h"

/* Operator precedence. The key represent operator type and the */
/* value represent operator precedence value. Some values are chosen. */
const std::map<char, int> InfixExpr::c_operatorPrecedence = {
     {'+', 90},  {'-', 90},  {'*', 100},
     {'/', 100}, {'^', 100}, {'%', 100}};

/* More than one digit number pose issue to separate operator operands. For this */
/* purpose is added posfix number delimiter. */
const char InfixExpr::c_postfixNumDelim = ' ';

/*
 * Constructor: InfixExpr
 * ---------------------
 * Default constructor.
 */

InfixExpr::InfixExpr()
    : m_infix(), m_postfix(), m_stack()
{
}

InfixExpr::InfixExpr(const char* a_infix)
    : m_infix(a_infix), m_postfix(), m_stack()
{
}

const InfixExpr& InfixExpr::operator=(const char* a_infix)
{
    m_infix = a_infix;
    m_postfix = std::string();

    // clear stack for new expression
    while (!m_stack.isEmpty()) {
        m_stack.pop();
    }

    return *this;
}

/*
 * Function: getInfix
 * ------------------
 * Function get infix notation.
 */

const std::string& InfixExpr::getInfix() const
{
    return m_infix;
}

/*
 * Function: getPostfix
 * --------------------
 * Function get postfix notation.
 */

const std::string& InfixExpr::getPostfix() const
{
    return m_postfix;
}

/*
 * Function: isOperator(op)
 * ------------------------
 * Function determines supported operators.
 */

bool InfixExpr::isOperator(char op) const
{
    /* supported list of operators */
    const auto result = c_operatorPrecedence.find(op);
    return (result != c_operatorPrecedence.cend());
}

/*
 * Function: precedence(op1, op2)
 * ------------------------------
 * Function determine operator `op1` precedence over operator `op2`.
 */

bool InfixExpr::precedence(char op1, char op2) const
{
    const auto op1Prec = c_operatorPrecedence.find(op1);
    const auto op2Prec = c_operatorPrecedence.find(op2);

    return ((op1Prec->second) >= (op2Prec->second));
}

/*
 * Function: convertToPostfix
 * --------------------------
 * Algorithm for conversion from infix to postfix notation. Whitespaces
 * are ignored.
 * Example:
 *   infix notation:   (6 + 2) * 5 - 8 / 4
 *   postfix notation:  6 2 + 5 * 8 4 / -
 */

void InfixExpr::convertToPostfix()
{
    // add left parenthesis to stack
    m_stack.push('(');

    // append right parenthesis to the end
    m_infix.push_back(')');

    for (auto it = m_infix.cbegin(); it != m_infix.cend(); ++it)
    {
        if (m_stack.isEmpty()) {
            return;
        }

        if (isdigit(*it))
        {
            // extract all digits and add postfix number delimiter
            while (isdigit(*it))
            {
                m_postfix.push_back(*it);
                ++it;
            }

            it = std::prev(it);
            m_postfix.push_back(c_postfixNumDelim);
        }
        else if (*it == '(') {
            m_stack.push(*it);
        }
        else if (isOperator(*it))
        {
            while (!m_stack.isEmpty())
            {
                // compare infix operator and stack operator precedence
                if (!precedence(m_stack.top(), *it))
                    break;

                char op;
                m_stack.pop(op);
                m_postfix.push_back(op);
            }

            // lower operator precedence
            m_stack.push(*it);
        }
        else if (*it == ')')
        {
            char op;
            while (m_stack.top() != '(') {
                m_stack.pop(op);
                m_postfix.push_back(op);
            }

            // remove left parenthesis
            m_stack.pop();
        }
    }
}
