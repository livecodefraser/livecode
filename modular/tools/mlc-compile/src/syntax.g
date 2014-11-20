'module' syntax

'use'
    types
    support

'export'
    InitializeSyntax
    GenerateSyntax

--------------------------------------------------------------------------------

'action' InitializeSyntax

    'rule' InitializeSyntax:
        -- do nothing

'sweep' GenerateSyntax(ANY)

    -- Handle the top-level definition
    'rule' GenerateSyntax(DEFINITION'syntax(_, _, Id, Class, Syntax, Methods)):
        Id'Name -> Name
        Id'Meaning -> syntax(Info)
        Info'Parent -> ParentId
        ParentId'Name -> ModuleName
        (|
            where(Class -> phrase)
            BeginPhraseSyntaxRule(ModuleName, Name)
        ||
            where(Class -> statement)
            BeginStatementSyntaxRule(ModuleName, Name)
        ||
            where(Class -> expression)
            BeginExpressionSyntaxRule(ModuleName, Name)
        ||
            where(Class -> prefix(Precedence))
            BeginPrefixOperatorSyntaxRule(ModuleName, Name, Precedence)
        ||
            where(Class -> postfix(Precedence))
            BeginPostfixOperatorSyntaxRule(ModuleName, Name, Precedence)
        ||
            where(Class -> binary(left, Precedence))
            BeginLeftBinaryOperatorSyntaxRule(ModuleName, Name, Precedence)
        ||
            where(Class -> binary(right, Precedence))
            BeginRightBinaryOperatorSyntaxRule(ModuleName, Name, Precedence)
        ||
            where(Class -> binary(neutral, Precedence))
            BeginNeutralBinaryOperatorSyntaxRule(ModuleName, Name, Precedence)
        |)

        BeginSyntaxGrammar()
        GenerateSyntax(Syntax)
        EndSyntaxGrammar()
        
        --BeginSyntaxMappings()
        --GenerateSyntax(Methods)
        --EndSyntaxMappings()

        EndSyntaxRule()

    -- Handle the syntax grammar part of the definition
    'rule' GenerateSyntax(SYNTAX'concatenate(_, Left, Right)):
        GenerateSyntax(Left)
        GenerateSyntax(Right)
        ConcatenateSyntaxGrammar()

    'rule' GenerateSyntax(SYNTAX'alternate(_, Left, Right)):
        GenerateSyntax(Left)
        GenerateSyntax(Right)
        AlternateSyntaxGrammar()

    'rule' GenerateSyntax(SYNTAX'optional(_, Operand)):
        GenerateSyntax(Operand)
        PushEmptySyntaxGrammar()
        AlternateSyntaxGrammar()

    'rule' GenerateSyntax(SYNTAX'repeat(_, Element)):
        GenerateSyntax(Element)
        PushEmptySyntaxGrammar()
        RepeatSyntaxGrammar()
        
    'rule' GenerateSyntax(SYNTAX'list(_, Element, Delimiter)):
        GenerateSyntax(Element)
        GenerateSyntax(Delimiter)
        RepeatSyntaxGrammar()
        
    'rule' GenerateSyntax(SYNTAX'keyword(_, Value)):
        PushKeywordSyntaxGrammar(Value)

    'rule' GenerateSyntax(SYNTAX'markedrule(_, Variable, Id)):
        Id'Name -> Name
        Variable'Meaning -> syntaxmark(Info)
        Info'Index -> Index
        Info'LMode -> LMode
        Info'RMode -> RMode
        MapMode(LMode -> LModeInt)
        MapMode(RMode -> RModeInt)
        PushMarkedDescentSyntaxGrammar(Index, Name, LModeInt, RModeInt)
        
    'rule' GenerateSyntax(SYNTAX'rule(_, Id)):
        Id'Name -> Name
        PushDescentSyntaxGrammar(Name)
        
    'rule' GenerateSyntax(SYNTAX'mark(_, Variable, Value)):
        Variable'Meaning -> syntaxmark(Info)
        Info'Index -> Index
        (|
            where(Value -> true(_))
            PushMarkedTrueSyntaxGrammar(Index)
        ||
            where(Value -> false(_))
            PushMarkedFalseSyntaxGrammar(Index)
        ||
            where(Value -> integer(_, IntValue))
            PushMarkedIntegerSyntaxGrammar(Index, IntValue)
        ||
            where(Value -> real(_, RealValue))
            PushMarkedRealSyntaxGrammar(Index, RealValue)
        ||
            where(Value -> string(_, StringValue))
            PushMarkedStringSyntaxGrammar(Index, StringValue)
        ||
            Fatal_InternalInconsistency("invalid constant type for marked variable")
        |)

/* This is handled when the module is compiled after syntax has been generated.
    -- Handle the syntax mapping part of the definition
    'rule' GenerateSyntax(SYNTAXMETHOD'method(_, Id, Arguments)):
        Id'Name -> Name
        BeginMethodSyntaxMapping(Name)
        GenerateSyntax(Arguments)
        EndMethodSyntaxMapping()
        
    'rule' GenerateSyntax(SYNTAXCONSTANT'undefined(_)):
        PushUndefinedArgumentSyntaxMapping()
        
    'rule' GenerateSyntax(SYNTAXCONSTANT'true(_)):
        PushTrueArgumentSyntaxMapping()
        
    'rule' GenerateSyntax(SYNTAXCONSTANT'false(_)):
        PushFalseArgumentSyntaxMapping()

    'rule' GenerateSyntax(SYNTAXCONSTANT'integer(_, Value)):
        PushIntegerArgumentSyntaxMapping(Value)

    'rule' GenerateSyntax(SYNTAXCONSTANT'real(_, Value)):
        PushRealArgumentSyntaxMapping(Value)

    'rule' GenerateSyntax(SYNTAXCONSTANT'string(_, Value)):
        PushStringArgumentSyntaxMapping(Value)

    'rule' GenerateSyntax(SYNTAXCONSTANT'variable(_, Value)):
        Value'Meaning -> syntaxmark(Info)
        Info'Index -> Index
        PushMarkArgumentSyntaxMapping(Index)

    'rule' GenerateSyntax(SYNTAXCONSTANT'indexedvariable(_, Value, _)):
        Value'Meaning -> syntaxmark(Info)
        Info'Index -> Index
        PushMarkArgumentSyntaxMapping(Index)
*/

'action' MapMode(MODE -> INT)
    'rule' MapMode(uncomputed -> 0):
    'rule' MapMode(in -> 0):
    'rule' MapMode(out -> 1):
    'rule' MapMode(inout -> 2):

/*'action' GenerateLModeOfMark(INT, MODE)
    'rule' GenerateLModeOfMark(Index, in):
        SetLModeOfMarkToIn(Index)
    'rule' GenerateLModeOfMark(Index, out):
        SetLModeOfMarkToOut(Index)
    'rule' GenerateLModeOfMark(Index, inout):
        SetLModeOfMarkToInOut(Index)

'action' GenerateRModeOfMark(INT, MODE)
    'rule' GenerateRModeOfMark(Index, in):
        SetRModeOfMarkToIn(Index)
    'rule' GenerateRModeOfMark(Index, out):
        SetRModeOfMarkToOut(Index)
    'rule' GenerateRModeOfMark(Index, inout):
        SetRModeOfMarkToInOut(Index)*/

--------------------------------------------------------------------------------
