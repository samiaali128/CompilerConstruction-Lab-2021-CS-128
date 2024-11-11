#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>

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
    T_Agar,
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
    T_EOF,
    T_RETURN,
    T_ELSE
};

struct Token
{
    TokenType type;
    string value;
    size_t line;
};

class Lexer
{
private:
    string src;
    size_t pos;
    size_t line;
    unordered_map<string, TokenType> keywords;

public:
    Lexer(const string &src) : pos(0), line(1), keywords({{"int", T_INT}, {"float", T_FLOAT}, {"double", T_DOUBLE}, {"string", T_STRING}, {"bool", T_BOOL}, {"char", T_CHAR}, {"Agar", T_Agar}, {"return", T_RETURN}, {"else", T_ELSE}})
    {
        this->src = src;
    }

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];
            if (isspace(current))
            {
                if (current == '\n')
                    line++;
                pos++;
                continue;
            }
            if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            }
            if (current == '"')
            {
                tokens.push_back(Token{T_STRING, consumeString(), line});
                continue;
            }
            if (isalpha(current))
            {
                string word = consumeWord();
                auto it = keywords.find(word);
                if (it != keywords.end())
                {
                    tokens.push_back(Token{it->second, word, line});
                }
                else
                {
                    tokens.push_back(Token{T_ID, word, line});
                }
                continue;
            }
            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", line});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", line});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", line});
                break;
            case ' ':
                tokens.push_back(Token{T_MUL, "", line});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", line});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", line});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", line});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", line});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", line});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", line});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", line});
                break;
            default:
                cerr << "Unexpected character: " << current << " at line " << line << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});
        return tokens;
    }

    string consumeNumber()
    {
        size_t start = pos;
        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.'))
            pos++;
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
        size_t start = pos;
        pos++;
        while (pos < src.size() && src[pos] != '"')
        {
            if (src[pos] == '\n')
                line++;
            pos++;
        }
        if (pos < src.size())
        {
            pos++;
            return src.substr(start, pos - start);
        }
        else
        {
            cerr << "Unterminated string literal at line " << line << endl;
            exit(1);
        }
    }
};

class Parser
{
private:
    vector<Token> tokens;
    size_t pos;

public:
    Parser(const vector<Token> &tokens) : tokens(tokens), pos(0) {}

    void parse()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
    }

    void parseStatement()
    {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT ||
            tokens[pos].type == T_DOUBLE || tokens[pos].type == T_STRING ||
            tokens[pos].type == T_BOOL || tokens[pos].type == T_CHAR)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_Agar)
        {
            parseAgarStatement();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else
        {
            cerr << "Syntax error: unexpected token type " << tokens[pos].type
                 << " at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseAgarStatement()
    {
        expect(T_Agar);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        expect(T_LBRACE);
        parseStatement();
        expect(T_RBRACE);

        if (tokens[pos].type == T_ELSE)
        {
            expect(T_ELSE);
            expect(T_LBRACE);
            parseStatement();
            expect(T_RBRACE);
        }
    }

    void expect(TokenType expected)
    {
        if (tokens[pos].type == expected)
        {
            pos++;
        }
        else
        {
            cerr << "Syntax error: expected token type " << expected
                 << " but found token type " << tokens[pos].type
                 << " at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseExpression()
    {
        if (tokens[pos].type == T_ID || tokens[pos].type == T_NUM)
        {
            pos++;
            if (tokens[pos].type == T_GT)
            { // Support for > comparison
                pos++;
                if (tokens[pos].type == T_ID || tokens[pos].type == T_NUM)
                {
                    pos++;
                }
                else
                {
                    cerr << "Expected an identifier or number after '>' at line " << tokens[pos].line << endl;
                    exit(1);
                }
            }
        }
        else
        {
            cerr << "Expected an identifier or number at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseDeclaration()
    {
        expect(tokens[pos].type);
        expect(T_ID);
        expect(T_SEMICOLON);
    }

    void parseAssignment()
    {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }
};

int main()
{
    string sourceCode = R"(
        int a;
        float b;

        a = 5;
        b = 3.14

        Agar (a > 10) {
            return a;
        } else {
            return b;
        }
    )";

    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parse();

    cout << "Parsing completed successfully!" << endl;
    return 0;
}