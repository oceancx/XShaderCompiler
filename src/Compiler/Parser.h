/*
 * Parser.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2016 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_PARSER_H
#define XSC_PARSER_H


#include <Xsc/Log.h>
#include "Scanner.h"
#include "HLSLErr.h"
#include "ASTEnums.h"
#include "ReportHandler.h"
#include "Visitor.h"
#include "Token.h"

#include <vector>
#include <map>
#include <string>
#include <stack>


namespace Xsc
{


// Syntax parser base class.
class Parser
{
    
    public:
        
        virtual ~Parser();

    protected:
        
        using Tokens        = Token::Types;
        using BinaryOpList  = std::initializer_list<BinaryOp>;

        /* === Functions === */

        Parser(Log* log);

        /* ----- Report Handling ----- */

        void Error(const std::string& msg, Token* tkn, const HLSLErr errorCode = HLSLErr::Unknown, bool breakWithExpection = true);
        void Error(const std::string& msg, bool prevToken = true, const HLSLErr errorCode = HLSLErr::Unknown, bool breakWithExpection = true);

        void ErrorUnexpected(const std::string& hint = "", Token* tkn = nullptr, bool breakWithExpection = true);
        void ErrorUnexpected(const Tokens type, Token* tkn = nullptr, bool breakWithExpection = true);

        void ErrorInternal(const std::string& msg, const std::string& procName);

        void Warning(const std::string& msg, Token* tkn);
        void Warning(const std::string& msg, bool prevToken = true);

        /* ----- Scanner ----- */

        virtual ScannerPtr MakeScanner() = 0;

        void PushScannerSource(const SourceCodePtr& source, const std::string& filename = "");
        bool PopScannerSource();

        // Returns the current token scanner.
        Scanner& GetScanner();

        // Returns the filename for the current scanner source.
        std::string GetCurrentFilename() const;

        TokenPtr Accept(const Tokens type);
        TokenPtr Accept(const Tokens type, const std::string& spell);
        TokenPtr AcceptIt();

        // Pushes the specified token string onto the stack where further tokens will be parsed from the top of the stack.
        void PushTokenString(const TokenPtrString& tokenString);
        void PopTokenString();

        // Ignores the next tokens if they are white spaces and optionally new lines.
        void IgnoreWhiteSpaces(bool includeNewLines = false);//, bool includeComments = true);
        void IgnoreNewLines();

        /* ----- Parsing ----- */

        // Builds a left-to-right binary-expression tree hierarchy for the specified list of expressions.
        ExprPtr BuildBinaryExprTree(std::vector<ExprPtr>& exprs, std::vector<BinaryOp>& ops);

        ExprPtr ParseGenericExpr();
        TernaryExprPtr ParseTernaryExpr(const ExprPtr& condExpr);

        ExprPtr ParseAbstractBinaryExpr(
            const std::function<ExprPtr()>& parseFunc,
            const BinaryOpList& binaryOps
        );

        ExprPtr ParseLogicOrExpr();
        ExprPtr ParseLogicAndExpr();
        ExprPtr ParseBitwiseOrExpr();
        ExprPtr ParseBitwiseXOrExpr();
        ExprPtr ParseBitwiseAndExpr();
        ExprPtr ParseEqualityExpr();
        ExprPtr ParseRelationExpr();
        ExprPtr ParseShiftExpr();
        ExprPtr ParseAddExpr();
        ExprPtr ParseSubExpr();
        ExprPtr ParseMulExpr();
        ExprPtr ParseDivExpr();
        ExprPtr ParseValueExpr();

        virtual ExprPtr ParsePrimaryExpr() = 0;

        /* ----- Common ----- */

        // Returns the log pointer or null if no log was defined.
        inline Log* GetLog() const
        {
            return log_;
        }

        // Returns the report handler.
        inline ReportHandler& GetReportHandler()
        {
            return reportHandler_;
        }

        // Makes a new shared pointer of the specified AST node class.
        template <typename T, typename... Args>
        std::shared_ptr<T> Make(Args&&... args)
        {
            return std::make_shared<T>(GetScanner().Pos(), args...);
        }

        // Returns the current token.
        inline const TokenPtr& Tkn() const
        {
            return tkn_;
        }

        // Returns the type of the next token.
        inline Tokens TknType() const
        {
            return Tkn()->Type();
        }

        // Returns true if the next token is from the specified type.
        inline bool Is(const Tokens type) const
        {
            return (TknType() == type);
        }

        // Returns true if the next token is from the specified type and has the specified spelling.
        inline bool Is(const Tokens type, const std::string& spell) const
        {
            return (TknType() == type && Tkn()->Spell() == spell);
        }

    private:

        /* === Structures === */

        struct ScannerStackEntry
        {
            ScannerPtr  scanner;
            std::string filename;
            TokenPtr    nextToken;
        };

        /* === Members === */

        ReportHandler                   reportHandler_;

        Log*                            log_            = nullptr;
        TokenPtr                        tkn_;

        std::stack<ScannerStackEntry>   scannerStack_;

};


} // /namespace Xsc


#endif



// ================================================================================