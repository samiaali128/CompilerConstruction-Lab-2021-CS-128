#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <regex>
#include <unordered_set>

using namespace std;

enum TokenType
{
    T_INT,
    T_FLOAT,
    T_DOUBLE,
    T_STRING,
    T_BOOL,
    T_CHAR,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_SEMICOLON,
    T_GT,
    T_LT,
    T_EOF,
    T_AGAR,
    T_MAGAR,
    T_WHILE,
    T_EQ,
    T_NE,
    T_LE,
    T_GE,
    T_FOR,
    T_SWITCH,
    T_CASE,
    T_BREAK,
    T_CONTINUE,
    T_PREPROCESSOR,
    T_UNKNOWN,
    T_TRUE,
    T_FALSE,
    T_COLON,
    T_DEFAULT,
    T_DO,
    T_STREAM_INSERTION_OPERATOR,
    T_STANDARD_OUTPUT_STREAM, // << , cout
    T_EXTRACTION_OPERATOR,
    T_STARNDARD_INPUT_STREAM, // >>, cin
    T_LOGICAL_AND,
    T_LOGICAL_OR,
    T_VOID
};

struct Token
{
    TokenType type;
    string value;
    int lineNumber;
};

class Lexer
{
private:
    string src;
    size_t pos;
    int lineNumber;

public:
    Lexer(const string &src) : src(src), pos(0), lineNumber(1) {}

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];
            // Handle new line
            if (current == '\n')
            {
                lineNumber++;
                pos++;
                continue;
            }
            // Handle spaces
            if (isspace(current))
            {
                pos++;
                continue;
            }
            // Hanlde Preprocessor Directives
            if (current == '#')
            {
                size_t start = pos;
                while (pos < src.size() && src[pos] != '\n')
                    pos++;
                string directive = src.substr(start, pos - start);
                tokens.push_back(Token{T_PREPROCESSOR, directive, lineNumber});
                continue;
            }

            // Handle ==
            if (current == '=' && pos + 1 < src.size() && src[pos + 1] == '=')
            {
                tokens.push_back(Token{T_EQ, "==", lineNumber});
                pos += 2;
                continue;
            }
            // Handle !=
            if (current == '!' && pos + 1 < src.size() && src[pos + 1] == '=')
            {
                tokens.push_back(Token{T_NE, "!=", lineNumber});
                pos += 2;
                continue;
            }
            // Handle <<
            if (current == '<' && pos + 1 < src.size() && src[pos + 1] == '<')
            {
                tokens.push_back(Token{T_STREAM_INSERTION_OPERATOR, "<<", lineNumber});
                pos += 2;
                continue;
            }
            // Handle >>
            if (current == '>' && pos + 1 < src.size() && src[pos + 1] == '>')
            {
                tokens.push_back(Token{T_EXTRACTION_OPERATOR, ">>", lineNumber});
                pos += 2;
                continue;
            }
            // Handle &&
            if (current == '&' && pos + 1 < src.size() && src[pos + 1] == '&')
            {
                tokens.push_back(Token{T_LOGICAL_AND, "&&", lineNumber});
                pos += 2;
                continue;
            }
            // Handle ||
            if (current == '|' && pos + 1 < src.size() && src[pos + 1] == '|')
            {
                tokens.push_back(Token{T_LOGICAL_OR, "||", lineNumber});
                pos += 2;
                continue;
            }

            // Handle <=
            if (current == '<' && pos + 1 < src.size() && src[pos + 1] == '=')
            {
                tokens.push_back(Token{T_LE, "<=", lineNumber});
                pos += 2;
                continue;
            }

            // Handle >=
            if (current == '>' && pos + 1 < src.size() && src[pos + 1] == '=')
            {
                tokens.push_back(Token{T_GE, ">=", lineNumber});
                pos += 2;
                continue;
            }

            // Check for comments, Single Line and MultiLine
            if (current == '/' && pos + 1 < src.size())
            {
                if (src[pos + 1] == '/' || src[pos + 1] == '*')
                {
                    skipComments();
                    continue;
                }
            }
            // Hanlde Int, Float, Double
            if (isdigit(current))
            {
                bool isFloat = false;
                string number = consumeNumber(isFloat);
                TokenType type = isFloat ? T_FLOAT : T_NUM;
                tokens.push_back(Token{type, number, lineNumber});
                continue;
            }

            // Handle String
            if (current == '"') // Start of a string
            {
                string strValue = consumeString();
                tokens.push_back(Token{T_STRING, strValue, lineNumber});
                continue;
            }
            // Handle Colon
            if (current == ':')
            {
                tokens.push_back(Token{T_COLON, ":", lineNumber});
                pos++;
                continue;
            }

            if (isalpha(current))
            {
                string word = consumeWord();
                TokenType type = T_ID;

                if (word == "int")
                    type = T_INT;
                else if (word == "float")
                    type = T_FLOAT;
                else if (word == "double")
                    type = T_DOUBLE;
                else if (word == "string")
                    type = T_STRING;
                else if (word == "char")
                    type = T_CHAR;
                else if (word == "bool")
                    type = T_BOOL;
                else if (word == "true")
                    type = T_TRUE;
                else if (word == "false")
                    type = T_FALSE;
                else if (word == "else")
                    type = T_ELSE;
                else if (word == "return")
                    type = T_RETURN;
                else if (word == "agar")
                    type = T_AGAR;
                else if (word == "if")
                    type = T_IF;
                else if (word == "magar")
                    type = T_MAGAR;
                else if (word == "while")
                    type = T_WHILE;
                else if (word == "for")
                    type = T_FOR;
                else if (word == "switch")
                    type = T_SWITCH;
                else if (word == "case")
                    type = T_CASE;
                else if (word == "break")
                    type = T_BREAK;
                else if (word == "continue")
                    type = T_CONTINUE;
                else if (word == "default")
                    type = T_DEFAULT;
                else if (word == "do")
                    type = T_DO;
                else if (word == "cout")
                    type = T_STANDARD_OUTPUT_STREAM;
                else if (word == "cin")
                    type = T_STARNDARD_INPUT_STREAM;
                else if (word == "void")
                    type = T_VOID;

                tokens.push_back(Token{type, word, lineNumber});
                continue;
            }

            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", lineNumber});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", lineNumber});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", lineNumber});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", lineNumber});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", lineNumber});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", lineNumber});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", lineNumber});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", lineNumber});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", lineNumber});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", lineNumber});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", lineNumber});
                break;
            case '<':
                tokens.push_back(Token{T_LT, "<", lineNumber});
                break;
            default:
                cout << "Unexpected character: " << current << " at line " << lineNumber << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", lineNumber});
        return tokens;
    }

    string consumeNumber(bool &isFloat)
    {
        size_t start = pos;
        bool hasDecimalPoint = false;
        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.'))
        {
            if (src[pos] == '.')
            {
                if (hasDecimalPoint)
                    break;
                hasDecimalPoint = true;
            }
            pos++;
        }
        if (pos < src.size() && (src[pos] == 'e' || src[pos] == 'E'))
        {
            isFloat = true;
            pos++;
            if (src[pos] == '+' || src[pos] == '-')
                pos++;
            while (pos < src.size() && isdigit(src[pos]))
                pos++;
        }
        isFloat = hasDecimalPoint || isFloat;
        return src.substr(start, pos - start);
    }

    string consumeWord()
    {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos]))
            pos++;
        return src.substr(start, pos - start);
    }

    string consumeString()
    {
        size_t start = ++pos; // Skip the initial quote (")
        string result;

        while (pos < src.size())
        {
            char current = src[pos];

            if (current == '\\' && pos + 1 < src.size()) // Handle escape sequences
            {
                char next = src[pos + 1];
                if (next == '"' || next == '\\') // Escaped quote or backslash
                {
                    result += next;
                    pos += 2;
                    continue;
                }
            }
            else if (current == '"') // End of string
            {
                pos++; // Move past the closing quote
                return result;
            }
            else
            {
                result += current;
                pos++;
            }
        }

        cout << "Unterminated string at line " << lineNumber << endl;
        exit(1); // Error: Unterminated string
    }

    void skipComments()
    {
        if (src[pos] == '/' && pos + 1 < src.size())
        {
            if (src[pos + 1] == '/')
            {
                pos += 2;
                while (pos < src.size() && src[pos] != '\n')
                    pos++;
            }
            else if (src[pos + 1] == '*')
            {
                pos += 2;
                while (pos < src.size() && !(src[pos] == '*' && pos + 1 < src.size() && src[pos + 1] == '/'))
                {
                    if (src[pos] == '\n')
                        lineNumber++;
                    pos++;
                }
                pos += 2; // Skip closing */
            }
        }
    }

    string tokenTypeToString(TokenType type)
    {
        switch (type)
        {
        case T_INT:
            return "INT";
        case T_FLOAT:
            return "FLOAT";
        case T_DOUBLE:
            return "DOUBLE";
        case T_STRING:
            return "STRING";
        case T_BOOL:
            return "BOOL";
        case T_TRUE:
            return "TRUE";
        case T_FALSE:
            return "FALSE";
        case T_CHAR:
            return "CHAR";
        case T_ID:
            return "IDENTIFIER";
        case T_NUM:
            return "NUMBER";
        case T_IF:
            return "IF";
        case T_ELSE:
            return "ELSE";
        case T_RETURN:
            return "RETURN";
        case T_ASSIGN:
            return "ASSIGN";
        case T_PLUS:
            return "PLUS";
        case T_MINUS:
            return "MINUS";
        case T_MUL:
            return "MULTIPLY";
        case T_DIV:
            return "DIVIDE";
        case T_LPAREN:
            return "LEFT_PAREN";
        case T_RPAREN:
            return "RIGHT_PAREN";
        case T_LBRACE:
            return "LEFT_BRACE";
        case T_RBRACE:
            return "RIGHT_BRACE";
        case T_SEMICOLON:
            return "SEMICOLON";
        case T_GT:
            return "GREATER_THAN";
        case T_LT:
            return "LESS_THAN";
        case T_EOF:
            return "EOF";
        case T_AGAR:
            return "AGAR";
        case T_MAGAR:
            return "MAGAR";
        case T_WHILE:
            return "WHILE";
        case T_EQ:
            return "EQUALS";
        case T_NE:
            return "NOT_EQUALS";
        case T_LE:
            return "LESS_OR_EQUAL";
        case T_GE:
            return "GREATER_OR_EQUAL";
        case T_FOR:
            return "FOR";
        case T_SWITCH:
            return "SWITCH";
        case T_CASE:
            return "CASE";
        case T_BREAK:
            return "BREAK";
        case T_CONTINUE:
            return "CONTINUE";
        case T_PREPROCESSOR:
            return "PREPROCESSOR";
        case T_COLON:
            return "COLON";
        case T_DEFAULT:
            return "DEFAULT";
        case T_DO:
            return "DO";
        case T_STREAM_INSERTION_OPERATOR:
            return "S_I_O";
        case T_STANDARD_OUTPUT_STREAM:
            return "COUT";
        case T_EXTRACTION_OPERATOR:
            return "E_O";
        case T_STARNDARD_INPUT_STREAM:
            return "CIN";
        case T_LOGICAL_AND:
            return "LOGICAL_AND";
        case T_LOGICAL_OR:
            return "LOGICAL_OR";
        case T_VOID:
            return "VOID";
        default:
            return "UNKNOWN";
        }
    }

    void printTokenizer(const vector<Token> &tokens)
    {
        // Clear the screen
        // system("cls");

        // Define column widths
        const int typeWidth = 20;
        const int valueWidth = 30;
        const int lineWidth = 10;

        // Print top border
        cout << "+" << string(typeWidth + 2, '-')
             << "+" << string(valueWidth + 2, '-')
             << "+" << string(lineWidth + 2, '-')
             << "+" << endl;

        // Print header row
        cout << "| " << left << setw(typeWidth) << "Token Type"
             << " | " << left << setw(valueWidth) << "Token Value"
             << " | " << left << setw(lineWidth) << "Line No."
             << " |" << endl;

        // Print header separator
        cout << "+" << string(typeWidth + 2, '-')
             << "+" << string(valueWidth + 2, '-')
             << "+" << string(lineWidth + 2, '-')
             << "+" << endl;

        // Check if tokens are empty
        if (tokens.empty())
        {
            cout << "| " << left << setw(typeWidth + valueWidth + lineWidth + 7)
                 << "No tokens available." << " |" << endl;
        }
        else
        {
            // Print each token
            for (const auto &token : tokens)
            {
                cout << "| " << left << setw(typeWidth) << tokenTypeToString(token.type)
                     << " | " << left << setw(valueWidth) << ("\"" + token.value + "\"")
                     << " | " << left << setw(lineWidth) << token.lineNumber
                     << " |" << endl;
            }
        }

        // Print bottom border
        cout << "+" << string(typeWidth + 2, '-')
             << "+" << string(valueWidth + 2, '-')
             << "+" << string(lineWidth + 2, '-')
             << "+" << endl;
    }
};

/*
    SymbolTable class:

    The SymbolTable class manages a collectin of variable names and their associated data types.
    It serves as a fundamental part of a compiler or interpreter, ensuring that variables are properly declared
    and accessed with their correct types during semantic analysis.


    Member Functions:
    1. declareVariable(const string &name, const string &type):
       - Purpose: Declares a new variable with a specified name and type.
       - This function checks if the variable is already declared. If it is, a runtime error is thrown indicating
         that the variable has already been declared.
       - If the variable is not already declared, it adds the variable's name as a key and its type as the value
         to the `symbolTable` map.
       - It will throw a runtime error if the variable already exists in the symbol table.
       - Example usage: Declare a new variable `x` with type `int`.

       if (symbolTable.find(name) != symbolTable.end()) {
           throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
       }
       symbolTable[name] = type;

    2. getVariableType(const string &name):
       - Purpose: Returns the type of a variable given its name.
       - This function checks if the variable exists in the `symbolTable`. If the variable is not found, it throws
         a runtime error indicating that the variable has not been declared yet.
       - If the variable is declared, it retrieves and returns its associated type from the symbol table.
       - Throws a runtime error if the variable is not found.
       - Example usage: Get the type of variable `x`.

       if (symbolTable.find(name) == symbolTable.end()) {
           throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
       }
       return symbolTable[name];

    3. isDeclared(const string &name) const:
       - Purpose: Checks whether a variable has been declared.
       - This function returns a boolean value indicating whether the variable exists in the `symbolTable`.
       - It is a quick way to check the existence of a variable without retrieving its type.
       - Example usage: Check if variable `x` is declared.

       return symbolTable.find(name) != symbolTable.end();

    Private Data Members:

    - map<string, string> symbolTable:
      - A `map` that stores variable names (strings) as keys and their associated types (strings) as values.
      - This map allows efficient lookups to check if a variable is declared and to retrieve its type.

    Usage in a Compiler or Interpreter:
    - The `SymbolTable` is crucial for ensuring that variables are used consistently and correctly in a program.
    - When a variable is declared, it is added to the symbol table, and its type is stored.
    - When a variable is referenced, the symbol table is consulted to ensure that it has been declared and to retrieve its type.
    - The symbol table helps detect semantic errors such as undeclared variables or redeclared variables.
*/
class SymbolTable
{
public:
    void declareVariable(const string &name, const string &type)
    {
        if (symbolTable.find(name) != symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
        }
        symbolTable[name] = type;
    }

    string getVariableType(const string &name)
    {
        if (symbolTable.find(name) == symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
        }
        return symbolTable[name];
    }

    bool isDeclared(const string &name) const
    {
        return symbolTable.find(name) != symbolTable.end();
    }

    void printSymbolTable() const
    {
        // Clear the screen
        // system("cls");

        // Define column widths
        const int nameWidth = 25;
        const int typeWidth = 20;

        // Print top border
        cout << "+" << string(nameWidth + 2, '-') << "+" << string(typeWidth + 2, '-') << "+" << endl;

        // Print header row
        cout << "| " << left << setw(nameWidth) << "Variable Name"
             << " | " << left << setw(typeWidth) << "Type"
             << " |" << endl;

        // Print header separator
        cout << "+" << string(nameWidth + 2, '-') << "+" << string(typeWidth + 2, '-') << "+" << endl;

        // Check if the symbol table is empty
        if (symbolTable.empty())
        {
            cout << "| " << setw(nameWidth + typeWidth + 3) << "No symbols declared." << " |" << endl;
        }
        else
        {
            // Print each symbol
            for (const auto &entry : symbolTable)
            {
                cout << "| " << left << setw(nameWidth) << entry.first
                     << " | " << left << setw(typeWidth) << entry.second
                     << " |" << endl;
            }
        }

        // Print bottom border
        cout << "+" << string(nameWidth + 2, '-') << "+" << string(typeWidth + 2, '-') << "+" << endl;
    }

private:
    map<string, string> symbolTable;
};

class IntermediateCodeGnerator
{
public:
    vector<string> instructions;
    int tempCount = 0;

    string newTemp()
    {
        return "t" + to_string(tempCount++);
    }

    void addInstruction(const string &instr)
    {
        instructions.push_back(instr);
    }

    void printInstructions()
    {
        // file open
        for (const auto &instr : instructions)
        {
            // write in file
            cout << instr << endl;
        }
    }

    void saveInstructionsToFile(const string &filename)
    {
        // Open a .obj file in text mode
        ofstream outFile(filename);
        if (!outFile.is_open())
        {
            cerr << "Error: Unable to open file for writing!" << endl;
            return;
        }

        // Write each instruction to the file
        for (const auto &instr : instructions)
        {
            outFile << instr << endl;
        }

        // Close the file
        outFile.close();
        cout << "Generated Intermediate Code is saved to file: " << filename << endl;
    }
};

class Parser
{
public:
    // Constructor
    Parser(const vector<Token> &tokens, SymbolTable &symTable, IntermediateCodeGnerator &icg)
        : tokens(tokens), pos(0), symTable(symTable), icg(icg) {}
    // here the private member of this class are being initalized with the arguments passed to this constructor

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
    }

private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable &symTable;
    IntermediateCodeGnerator &icg;

    void parseStatement()
    {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT ||
            tokens[pos].type == T_DOUBLE || tokens[pos].type == T_STRING ||
            tokens[pos].type == T_CHAR || tokens[pos].type == T_BOOL)
        {
            parseDeclarationOrDeclarationAssignment();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_VOID)
        {
            parseVoidFunction();
        }
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_SWITCH)
        {
            parseSwitchStatement();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else if (tokens[pos].type == T_AGAR)
        {
            parseAgarStatement();
        }
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileStatement();
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForStatement();
        }
        else if (tokens[pos].type == T_BREAK)
        {
            parseBreakStatement();
        }
        else if (tokens[pos].type == T_DO)
        {
            parseDoWhileStatement();
        }
        else if (tokens[pos].type == T_STANDARD_OUTPUT_STREAM)
        {
            parsePrintStatement();
        }
        else if (tokens[pos].type == T_STARNDARD_INPUT_STREAM)
        {
            parseInputStatement();
        }
        else
        {
            cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
    }

    void parseVoidFunction()
    {
        cout << "LEts see\n";
        expect(T_VOID);
        expect(T_ID);
        expect(T_LPAREN);
        expect(T_RPAREN);
        parseBlock();
    }
    void parseInputStatement()
    {
        expect(T_STARNDARD_INPUT_STREAM);
        expect(T_EXTRACTION_OPERATOR);
        expect(T_ID);
        expect(T_SEMICOLON);
    }
    void parsePrintStatement()
    {
        expect(T_STANDARD_OUTPUT_STREAM);
        expect(T_STREAM_INSERTION_OPERATOR);
        expect(T_STRING);
        expect(T_SEMICOLON);
    }
    void parseDoWhileStatement()
    {
        // Parse 'do' keyword
        expect(T_DO);

        // Generate start label for the do-while loop
        string startLabel = icg.newTemp() + "_do_while_start";
        icg.addInstruction(startLabel + ":");

        // Parse the body of the do-while loop
        parseBlock();

        // Expect 'while' keyword
        expect(T_WHILE);
        expect(T_LPAREN);

        // Generate label for condition check
        string conditionLabel = icg.newTemp() + "_do_while_condition";
        icg.addInstruction(conditionLabel + ":");

        // Parse the condition expression
        string conditionExpr = parseExpression();
        expect(T_RPAREN);
        expect(T_SEMICOLON);

        // Generate condition check instruction
        string conditionTemp = icg.newTemp();
        icg.addInstruction(conditionTemp + " = " + conditionExpr);

        // Generate conditional jump back to start of loop
        string endLabel = icg.newTemp() + "_do_while_end";
        icg.addInstruction("if !" + conditionTemp + " goto " + endLabel);
        icg.addInstruction("goto " + startLabel);
        icg.addInstruction(endLabel + ":");
    }

    void parseBreakStatement()
    {
        expect(T_BREAK);
        expect(T_SEMICOLON);
    }
    void parseSwitchStatement()
    {
        // Parse 'switch' keyword
        expect(T_SWITCH);
        expect(T_LPAREN);

        // Parse the switch expression (what we're switching on)
        string switchExpr = parseExpression();
        expect(T_RPAREN);

        // Expect opening brace of switch block
        expect(T_LBRACE);

        // Flag to track if default case has been seen
        bool hasDefaultCase = false;

        // Generate labels for switch statement
        string endSwitchLabel = icg.newTemp() + "_switch_end";

        // Parse cases
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            if (tokens[pos].type == T_CASE)
            {
                // Parse case
                expect(T_CASE);

                // Parse case expression (can be a literal or constant expression)
                string caseExpr = parseExpression();

                // Expect colon after case
                expect(T_COLON);

                // Generate a unique label for this case
                string caseLabel = icg.newTemp() + "_case";

                // Add comparison instruction
                string compareTemp = icg.newTemp();
                icg.addInstruction(compareTemp + " = " + switchExpr + " == " + caseExpr);

                // Add conditional jump
                string nextCaseLabel = icg.newTemp() + "_next_case";
                icg.addInstruction("if !" + compareTemp + " goto " + nextCaseLabel);

                // Parse statements in this case block
                while (tokens[pos].type != T_CASE &&
                       tokens[pos].type != T_DEFAULT &&
                       tokens[pos].type != T_RBRACE)
                {
                    parseStatement();
                }

                // Add unconditional jump to end of switch
                icg.addInstruction("goto " + endSwitchLabel);

                // Label for next case
                icg.addInstruction(nextCaseLabel + ":");
            }
            else if (tokens[pos].type == T_DEFAULT)
            {
                // Ensure only one default case
                if (hasDefaultCase)
                {
                    cout << "Syntax error: Multiple default cases in switch statement" << endl;
                    exit(1);
                }

                expect(T_DEFAULT);
                expect(T_COLON);

                hasDefaultCase = true;

                // Parse statements in default case block
                while (tokens[pos].type != T_RBRACE)
                {
                    parseStatement();
                }
            }
            else
            {
                break;
            }
        }

        // Add end switch label
        icg.addInstruction(endSwitchLabel + ":");

        // Close switch block
        expect(T_RBRACE);
    }

    string parseIncrementDecrement()
    {
        if (tokens[pos].type == T_ID)
        {
            string var = tokens[pos].value;
            expect(T_ID);
            if (tokens[pos].type == T_PLUS && tokens[pos + 1].type == T_PLUS)
            {
                string incrementCode = var + " = " + var + " + 1"; // TAC for increment
                icg.addInstruction(incrementCode);
                pos += 2;
                return incrementCode;
            }
            else if (tokens[pos].type == T_MINUS && tokens[pos + 1].type == T_MINUS)
            {
                string decrementCode = var + " = " + var + " - 1"; // icg for decrement
                icg.addInstruction(decrementCode);
                pos += 2;
                return decrementCode;
            }
            else if (tokens[pos].type == T_ASSIGN)
            {
                pos++;
                string expr = parseExpression();
                string assignmentCode = var + " = " + expr; // icg for assignment
                icg.addInstruction(assignmentCode);
                return assignmentCode; // Return the icg for assignment
            }
            else
            {
                cout << "Syntax error: invalid increment/decrement in 'for' loop at line " << tokens[pos].lineNumber << endl;
                exit(1);
            }
        }
        else
        {
            cout << "Syntax error: expected increment/decrement expression in 'for' loop at lineNumber " << tokens[pos].lineNumber << endl;
            exit(1);
        }
    }

    void parseInitialization()
    {
        if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else
        {
            cout << "Syntax error: expected initialization statement in 'for' loop at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
    }

    void parseForStatement()
    {
        string initLabel = "L" + to_string(icg.tempCount++);
        string startLabel = "L" + to_string(icg.tempCount++);
        string endLabel = "L" + to_string(icg.tempCount++);

        // for (i = 0; i < 5; i = i + 1){}
        expect(T_FOR);
        expect(T_LPAREN);

        // parseInitialization();
        parseDeclarationOrDeclarationAssignment();
        icg.addInstruction(initLabel + ":");

        string condition = parseExpression();
        icg.addInstruction("if " + condition + " goto " + endLabel);

        expect(T_SEMICOLON);
        string incrementCode = parseIncrementDecrement();

        expect(T_RPAREN);

        icg.addInstruction(startLabel + ":");
        parseBlock(); // Parse the body of the loop
        icg.addInstruction(incrementCode);
        icg.addInstruction("goto " + initLabel);
        icg.addInstruction(endLabel + ":");
    }

    void parseWhileStatement()
    {
        string startLabel = "L" + to_string(icg.tempCount++);
        string endLabel = "L" + to_string(icg.tempCount++);

        icg.addInstruction("goto " + startLabel);
        icg.addInstruction(startLabel + ":");

        expect(T_WHILE);
        expect(T_LPAREN);
        string condition = parseExpression();
        expect(T_RPAREN);

        string temp = icg.newTemp();
        icg.addInstruction(temp + " = " + condition);

        icg.addInstruction("if " + temp + " goto " + endLabel);
        icg.addInstruction("goto " + startLabel);

        parseBlock();

        icg.addInstruction("goto " + startLabel);
        icg.addInstruction(endLabel + ":");
    }

    void parseAgarStatement()
    {
        expect(T_AGAR);
        expect(T_LPAREN);
        string cond = parseExpression();
        expect(T_RPAREN);

        string temp = icg.newTemp();
        icg.addInstruction(temp + " = " + cond);
        icg.addInstruction("agar " + temp + " goto L1");
        icg.addInstruction("goto L2");
        icg.addInstruction("L1:");

        parseStatement();

        if (tokens[pos].type == T_MAGAR)
        { // If an `magar` part exists, handle it.
            icg.addInstruction("goto L3");
            icg.addInstruction("L2:");
            expect(T_MAGAR);
            parseStatement();
            icg.addInstruction("L3:");
        }
        else
        {
            icg.addInstruction("L2:");
        }
    }

    /*
     parseDeclarationOrDeclarationAssignment handles the parsing of variable declarations and intializationat same line.
     It expects the token type to be `T_INT` (for declaring an integer type variable),
     followed by an identifier (variable name), and a semicolon to terminate the statement.
     It also registers the declared variable in the symbol table with type "int".
     Example:
     int x;   // This will be parsed and the symbol table will store x with type "int".
    */

    void parseDeclarationOrDeclarationAssignment()
    {
        // Determine the type of the variable
        string varType;
        switch (tokens[pos].type)
        {
        case T_INT:
            varType = "int";
            break;
        case T_FLOAT:
            varType = "float";
            break;
        case T_DOUBLE:
            varType = "double";
            break;
        case T_STRING:
            varType = "string";
            break;
        case T_CHAR:
            varType = "char";
            break;
        case T_BOOL:
            varType = "bool";
            break;
        default:
            cout << "Unexpected type in declaration" << endl;
            exit(1);
        }

        // Consume the type token
        expect(tokens[pos].type);

        // Get the variable name
        string varName = expectAndReturnValue(T_ID);

        // Declare the variable in the symbol table
        symTable.declareVariable(varName, varType);

        // Check if this is a declaration with assignment
        if (tokens[pos].type == T_ASSIGN)
        {
            // Consume the assignment token
            expect(T_ASSIGN);

            // Handle different types of assignments
            if (tokens[pos].type == T_STRING)
            {
                string strValue = expectAndReturnValue(T_STRING);
                icg.addInstruction(varName + " = " + strValue);
            }
            else if (tokens[pos].type == T_TRUE || tokens[pos].type == T_FALSE)
            {
                // Handle boolean literals
                string boolValue = expectAndReturnValue(tokens[pos].type);
                icg.addInstruction(varName + " = " + boolValue);
            }
            else
            {
                string expr = parseExpression();
                icg.addInstruction(varName + " = " + expr);
            }
        }

        // Expect semicolon to end the statement
        expect(T_SEMICOLON);
    }

    /*
    parseAssignment handles the parsing of variable assignments.
    It expects an identifier (variable name), an assignment operator `=`, followed by an expression,
    and a semicolon at the end of the statement.
    It checks if the variable is declared in the symbol table, parses the expression, and generates intermediate code
    for the assignment.
    Example:
    x = 10;   -->  This will be parsed, checking if x is declared, then generating intermediate code like `x = 10`.
   */

    // The parseAssignment function
    void parseAssignment()
    {
        string varName = expectAndReturnValue(T_ID);
        symTable.getVariableType(varName);
        expect(T_ASSIGN);

        if (tokens[pos].type == T_TRUE || tokens[pos].type == T_FALSE)
        {
            // Handling of boolean literals
            string boolValue = expectAndReturnValue(tokens[pos].type);
            icg.addInstruction(varName + " = " + boolValue);
        }
        else if (tokens[pos].type == T_STRING)
        {
            string strValue = expectAndReturnValue(T_STRING);
            icg.addInstruction(varName + " = " + strValue);
        }

        else
        {
            string expr = parseExpression();
            icg.addInstruction(varName + " = " + expr);
        }
        expect(T_SEMICOLON);
    }

    /*
        parseIfStatement handles the parsing of `if` statements.
        It expects the keyword `if`, followed by an expression in parentheses that serves as the condition.
        If the condition evaluates to true, it executes the statement inside the block. If an `else` part is present,
        it executes the corresponding statement after the `else` keyword.
        Intermediate code for the `if` statement is generated, including labels for conditional jumps.
        Example:
        if(5 > 3) { x = 20; }  --> This will generate intermediate code for the condition check and jump instructions.
   */

    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);
        string cond = parseExpression();
        expect(T_RPAREN);

        string temp = icg.newTemp();
        icg.addInstruction(temp + " = " + cond);

        icg.addInstruction("if " + temp + " goto L1");
        icg.addInstruction("goto L2");
        icg.addInstruction("L1:");

        parseStatement();

        if (tokens[pos].type == T_ELSE)
        { // If an `else` part exists, handle it.
            icg.addInstruction("goto L3");
            icg.addInstruction("L2:");
            expect(T_ELSE);
            parseStatement();
            icg.addInstruction("L3:");
        }
        else
        {
            icg.addInstruction("L2:");
        }
    }

    /*
        parseReturnStatement handles the parsing of `return` statements.
        It expects the keyword `return`, followed by an expression to return, and a semicolon to terminate the statement.
        It generates intermediate code to represent the return of the expression.
        Example:
        return x + 5;   -->  This will generate intermediate code like `return x + 5`.
    */

    void parseReturnStatement()
    {
        expect(T_RETURN);
        string expr = parseExpression();
        icg.addInstruction("return " + expr);
        expect(T_SEMICOLON);
    }

    /*
        parseBlock handles the parsing of block statements, which are enclosed in curly braces `{ }`.
        It parses the statements inside the block recursively until it reaches the closing brace.
        Example:
        { x = 10; y = 20; }   -->  This will parse each statement inside the block.
    */

    void parseBlock()
    {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    /*
       parseExpression handles the parsing of expressions involving addition, subtraction, or comparison operations.
       It first parses a term, then processes addition (`+`) or subtraction (`-`) operators if present, generating
       intermediate code for the operations.
       Example:
       5 + 3 - 2;  -->  This will generate intermediate code like `t0 = 5 + 3` and `t1 = t0 - 2`.
   */

    string parseExpression()
    {
        string term = parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS)
        {
            TokenType op = tokens[pos++].type;
            string nextTerm = parseTerm();
            string temp = icg.newTemp();
            icg.addInstruction(temp + " = " + term + (op == T_PLUS ? " + " : " - ") + nextTerm);
            term = temp;
        }
        while (tokens[pos].type == T_GT || tokens[pos].type == T_LT || tokens[pos].type == T_EQ || tokens[pos].type == T_NE || tokens[pos].type == T_LE || tokens[pos].type == T_GE || tokens[pos].type == T_LOGICAL_AND || tokens[pos].type == T_LOGICAL_OR)
        {
            // pos++;
            TokenType op = tokens[pos++].type;
            string nextExpr = parseExpression();
            string temp = icg.newTemp();
            if (op == T_GT)
            {
                icg.addInstruction(temp + " = " + term + " > " + nextExpr);
            }
            else if (op == T_EQ)
            {
                icg.addInstruction(temp + " = " + term + " == " + nextExpr);
            }
            else if (op == T_LT)
            {
                icg.addInstruction(temp + " = " + term + " < " + nextExpr);
            }
            else if (op == T_NE)
            {
                icg.addInstruction(temp + " = " + term + " != " + nextExpr);
            }
            else if (op == T_LE)
            {
                icg.addInstruction(temp + " = " + term + " <= " + nextExpr);
            }
            else if (op == T_GE)
            {
                icg.addInstruction(temp + " = " + term + " >= " + nextExpr);
            }
            else if (op == T_LOGICAL_AND)
            {
                icg.addInstruction(temp + " = " + term + " && " + nextExpr);
            }
            else if (op == T_LOGICAL_OR)
            {
                icg.addInstruction(temp + " = " + term + " || " + nextExpr);
            }

            term = temp;
        }
        return term;
    }

    /*
       parseTerm handles the parsing of terms involving multiplication or division operations.
       It first parses a factor, then processes multiplication (`*`) or division (`/`) operators if present,
       generating intermediate code for the operations.
       Example:
       5 * 3 / 2;   This will generate intermediate code like `t0 = 5 * 3` and `t1 = t0 / 2`.
   */

    string parseTerm()
    {
        string factor = parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            TokenType op = tokens[pos++].type;
            string nextFactor = parseFactor();
            string temp = icg.newTemp();
            icg.addInstruction(temp + " = " + factor + (op == T_MUL ? " * " : " / ") + nextFactor);
            factor = temp;
        }
        return factor;
    }

    /*
       parseFactor handles the parsing of factors in expressions, which can be either numeric literals, identifiers
       (variables), or expressions inside parentheses (for sub-expressions).
       Example:
       5;          -->  This will return the number "5".
       x;          -->  This will return the identifier "x".
       (5 + 3);    --> This will return the sub-expression "5 + 3".
   */

    string parseFactor()
    {
        if (tokens[pos].type == T_NUM)
        {
            return tokens[pos++].value;
        }
        // Handle float literals, e.g., "20.09774"
        else if (tokens[pos].type == T_FLOAT)
        {
            return tokens[pos++].value;
        }
        else if (tokens[pos].type == T_ID)
        {
            return tokens[pos++].value;
        }
        else if (tokens[pos].type == T_TRUE || tokens[pos].type == T_FALSE)
        {
            return tokens[pos++].value;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            string expr = parseExpression();
            expect(T_RPAREN);
            return expr;
        }
        else
        {
            cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
    }

    /*
       expect function:
       This functin is used to check whether the current token matches the expected type.
       If the token type does not match the expected type, an error message is displayed
       and the program exits. If the token type matches, it advances the position to the next token.
   */

    void expect(TokenType type)
    {
        if (tokens[pos].type != type)
        {
            cout << "Syntax error: expected '" << type << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
        pos++;
    }

    /*
   Explanation:
   - The `expect` function ensures that the parser encounters the correct tokens in the expected order.
   - It's mainly used for non-value-based tokens, such as keywords, operators, and delimiters (e.g., semicolons).
   - If the parser encounters an unexpected token, it halts the process by printing an error message, indicating where the error occurred (line number) and what was expected.
   - The `pos++` advances to the next token after confirming the expected token is present.

   Use Case:
   - This function is helpful when checking for the correct syntax or structure in a language's grammar, ensuring the parser processes the tokens in the correct order.
   */

    string expectAndReturnValue(TokenType type)
    {
        string value = tokens[pos].value;
        expect(type);
        return value;
    }

    /*
       Why both functions are needed:
       - The `expect` function is useful when you are only concerned with ensuring the correct token type without needing its value.
       - For example, ensuring a semicolon `;` or a keyword `if` is present in the source code.
       - The `expectAndReturnValue` function is needed when the parser not only needs to check for a specific token but also needs to use the value of that token in the next stages of compilation or interpretation.
       - For example, extracting the name of a variable (`T_ID`) or the value of a constant (`T_NUMBER`) to process it in a symbol table or during expression evaluation.
   */
};

class AssemblyCodeGenerator
{
public:
    vector<string> assemblyCode;
    unordered_set<string> definedVariables;

    void generateAssembly(const vector<string> &tacInstructions)
    {
        // Start with necessary assembly directives
        // assemblyCode.push_back("%include 'syscall.asm'  ; Include system call definitions");
        assemblyCode.push_back("section .data");
        // assemblyCode.push_back("    SYS_EXIT equ 1");
        // assemblyCode.push_back("    SYS_WRITE equ 4");
        // assemblyCode.push_back("    STDOUT equ 1");

        // Collect and declare variables
        collectVariables(tacInstructions);

        // Start text section
        assemblyCode.push_back("\nsection .text");
        assemblyCode.push_back("    global _start");
        assemblyCode.push_back("_start:");

        // Process each TAC instruction
        for (const auto &instr : tacInstructions)
        {
            if (instr.find(" = ") != string::npos)
            {
                processAssignment(instr);
            }
            else if (instr.find("if ") != string::npos || instr.find("agar ") != string::npos)
            {
                processConditional(instr);
            }
            else if (instr.find("goto") != string::npos)
            {
                processGoto(instr);
            }
            else if (instr.find(":") != string::npos)
            {
                processLabel(instr);
            }
            else if (!instr.empty())
            {
                cerr << "Unsupported TAC instruction: " << instr << endl;
            }
        }

        // Add program exit
        // addProgramExit();
    }

    void printAssembly() const
    {
        for (const auto &line : assemblyCode)
        {
            cout << line << endl;
        }
    }

    void saveInstructionsToFile(const string &filename)
    {
        // Open a .obj file in text mode
        ofstream outFile(filename);
        if (!outFile.is_open())
        {
            cerr << "Error: Unable to open file for writing!" << endl;
            return;
        }

        // Write each instruction to the file
        for (const auto &instr : assemblyCode)
        {
            outFile << instr << endl;
        }

        // Close the file
        outFile.close();
        cout << "Generated Assembly Code is saved to file: " << filename << endl;
    }

private:
    void collectVariables(const vector<string> &instructions)
    {
        for (const auto &instr : instructions)
        {
            // Extract variables from assignments and conditions
            extractVariablesFromInstruction(instr);
        }

        // Declare collected variables
        for (const auto &var : definedVariables)
        {
            if (isNumeric(var))
            {
                // Skip numeric constants
                continue;
            }
            assemblyCode.push_back("    " + var + " dd 0");
        }
    }

    void extractVariablesFromInstruction(const string &instr)
    {
        // Split instruction into tokens
        istringstream iss(instr);
        string token;
        while (iss >> token)
        {
            // Ignore keywords and operators
            if (token == "=" || token == "if" || token == "goto" ||
                token == "agar" || token == "+" || token == "-" ||
                token == "*" || token == "/" ||
                token.back() == ':')
            {
                continue;
            }

            // Skip if token is a numeric constant
            if (!isNumeric(token))
            {
                definedVariables.insert(token);
            }
        }
    }

    bool isNumeric(const string &str)
    {
        return str.find_first_not_of("0123456789") == string::npos;
    }

    void processAssignment(const string &instr)
    {
        size_t eqPos = instr.find(" = ");
        string lhs = instr.substr(0, eqPos);
        string rhs = instr.substr(eqPos + 3);

        if (rhs.find("+") != string::npos)
            translateBinaryOp(lhs, rhs, "add");
        else if (rhs.find("-") != string::npos)
            translateBinaryOp(lhs, rhs, "sub");
        else if (rhs.find("*") != string::npos)
            translateBinaryOp(lhs, rhs, "imul");
        else if (rhs.find("/") != string::npos)
            translateBinaryOp(lhs, rhs, "idiv");
        else
        {
            // Simple assignment or constant
            assemblyCode.push_back("    mov dword [" + lhs + "], " + rhs);
        }
    }

    void translateBinaryOp(const string &lhs, const string &rhs, const string &op)
    {
        size_t opPos = rhs.find_first_of("+-*/");
        string op1 = rhs.substr(0, opPos - 1);
        string op2 = rhs.substr(opPos + 2);

        assemblyCode.push_back("    mov eax, [" + op1 + "]");

        if (op == "idiv")
        {
            // Division requires special handling
            assemblyCode.push_back("    mov ebx, [" + op2 + "]");
            assemblyCode.push_back("    cdq  ; Sign extend for division");
            assemblyCode.push_back("    idiv ebx");
        }
        else
        {
            if (op == "imul")
            {
                assemblyCode.push_back("    imul eax, [" + op2 + "]");
            }
            else
            {
                // Addition or subtraction
                assemblyCode.push_back("    " + op + " eax, [" + op2 + "]");
            }
        }

        assemblyCode.push_back("    mov dword [" + lhs + "], eax");
    }

    void processConditional(const string &instr)
    {
        // Support for both 'if' and 'agar' keywords
        size_t condStart = (instr.find("if ") != string::npos) ? instr.find("if ") : instr.find("agar ");
        size_t gotoPos = instr.find("goto");
        string condition = instr.substr(condStart + 3, gotoPos - condStart - 3);
        string label = instr.substr(gotoPos + 5);

        if (condition.find_first_of("<>=") == string::npos)
        {
            // Boolean condition
            assemblyCode.push_back("    cmp dword [" + condition + "], 1");
            assemblyCode.push_back("    je " + label);
        }
        else
        {
            // Relational condition
            size_t opPos = condition.find_first_of("<>=");
            string op1 = condition.substr(0, opPos - 1);
            string op2 = condition.substr(opPos + 2);
            string compOp = condition.substr(opPos, 1);

            assemblyCode.push_back("    mov eax, [" + op1 + "]");
            assemblyCode.push_back("    cmp eax, [" + op2 + "]");

            if (compOp == ">")
                assemblyCode.push_back("    jg " + label);
            else if (compOp == "<")
                assemblyCode.push_back("    jl " + label);
            else if (compOp == "=")
                assemblyCode.push_back("    je " + label);
        }
    }

    void processLabel(const string &instr)
    {
        // Remove whitespace and ensure proper label format
        string trimmedLabel = regex_replace(instr, regex("^\\s+|\\s+$"), "");
        assemblyCode.push_back("\n" + trimmedLabel);
    }

    void processGoto(const string &instr)
    {
        size_t gotoPos = instr.find("goto");
        string label = instr.substr(gotoPos + 5);
        assemblyCode.push_back("    jmp " + label);
    }

    void addProgramExit()
    {
        // Add standard exit syscall
        assemblyCode.push_back("\n    ; Exit program");
        assemblyCode.push_back("    mov eax, SYS_EXIT");
        assemblyCode.push_back("    xor ebx, ebx  ; Exit code 0");
        assemblyCode.push_back("    int 0x80");
    }
};

int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is provided
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // Open the file
    ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    // // Read and print the file content
    string input = "", line;
    while (getline(inputFile, line))
    {
        input += line + "\n";
    }

    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();

    lexer.printTokenizer(tokens);

    SymbolTable symTable;
    IntermediateCodeGnerator icg;
    Parser parser(tokens, symTable, icg);

    parser.parseProgram();
    symTable.printSymbolTable();

    // cout << "\nThree Address Code:" << endl;
    // icg.printInstructions();
    icg.saveInstructionsToFile("./icg.obj");

    AssemblyCodeGenerator acg;
    acg.generateAssembly(icg.instructions);

    // cout << "\nAssembly Code:" << endl;
    // acg.printAssembly();
    acg.saveInstructionsToFile("./assembly.asm");

    return 0;
}
