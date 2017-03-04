#include <tree_sitter/parser.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#define LANGUAGE_VERSION 1
#define STATE_COUNT 16
#define SYMBOL_COUNT 11
#define TOKEN_COUNT 8
#define EXTERNAL_TOKEN_COUNT 0

enum {
    anon_sym_LPAREN = 1,
    anon_sym_RPAREN = 2,
    anon_sym_PLUS = 3,
    anon_sym_STAR = 4,
    sym_number = 5,
    sym_comment = 6,
    sym_variable = 7,
    sym_expression = 8,
    sym_sum = 9,
    sym_product = 10,
};

static const char *ts_symbol_names[] = {
    [ts_builtin_sym_end] = "END",
    [anon_sym_LPAREN] = "(",
    [anon_sym_RPAREN] = ")",
    [anon_sym_PLUS] = "+",
    [anon_sym_STAR] = "*",
    [sym_number] = "number",
    [sym_comment] = "comment",
    [sym_variable] = "variable",
    [sym_expression] = "expression",
    [sym_sum] = "sum",
    [sym_product] = "product",
};

static const TSSymbolMetadata ts_symbol_metadata[SYMBOL_COUNT] = {
    [ts_builtin_sym_end] = {
        .visible = false,
        .named = true,
        .structural = true,
        .extra = false,
    },
    [anon_sym_LPAREN] = {
        .visible = true,
        .named = false,
        .structural = true,
        .extra = false,
    },
    [anon_sym_RPAREN] = {
        .visible = true,
        .named = false,
        .structural = true,
        .extra = false,
    },
    [anon_sym_PLUS] = {
        .visible = true,
        .named = false,
        .structural = true,
        .extra = false,
    },
    [anon_sym_STAR] = {
        .visible = true,
        .named = false,
        .structural = true,
        .extra = false,
    },
    [sym_number] = {
        .visible = true,
        .named = true,
        .structural = true,
        .extra = false,
    },
    [sym_comment] = {
        .visible = true,
        .named = true,
        .structural = false,
        .extra = true,
    },
    [sym_variable] = {
        .visible = true,
        .named = true,
        .structural = true,
        .extra = false,
    },
    [sym_expression] = {
        .visible = true,
        .named = true,
        .structural = true,
        .extra = false,
    },
    [sym_sum] = {
        .visible = true,
        .named = true,
        .structural = true,
        .extra = false,
    },
    [sym_product] = {
        .visible = true,
        .named = true,
        .structural = true,
        .extra = false,
    },
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
    START_LEXER();
    switch (state) {
        case 0:
            if (lookahead == 0)
                ADVANCE(1);
            if ((lookahead == '\t') ||
                (lookahead == '\n') ||
                (lookahead == '\r') ||
                (lookahead == ' '))
                SKIP(0);
            if (lookahead == '#')
                ADVANCE(2);
            if (lookahead == '(')
                ADVANCE(3);
            if (lookahead == ')')
                ADVANCE(4);
            if (lookahead == '*')
                ADVANCE(5);
            if (lookahead == '+')
                ADVANCE(6);
            if ('0' <= lookahead && lookahead <= '9')
                ADVANCE(7);
            if (('A' <= lookahead && lookahead <= 'Z') ||
                ('a' <= lookahead && lookahead <= 'z'))
                ADVANCE(8);
            LEX_ERROR();
        case 1:
            ACCEPT_TOKEN(ts_builtin_sym_end);
        case 2:
            if (!((lookahead == 0) ||
                (lookahead == '\n')))
                ADVANCE(2);
            ACCEPT_TOKEN(sym_comment);
        case 3:
            ACCEPT_TOKEN(anon_sym_LPAREN);
        case 4:
            ACCEPT_TOKEN(anon_sym_RPAREN);
        case 5:
            ACCEPT_TOKEN(anon_sym_STAR);
        case 6:
            ACCEPT_TOKEN(anon_sym_PLUS);
        case 7:
            if ('0' <= lookahead && lookahead <= '9')
                ADVANCE(7);
            ACCEPT_TOKEN(sym_number);
        case 8:
            if (('0' <= lookahead && lookahead <= '9') ||
                ('A' <= lookahead && lookahead <= 'Z') ||
                (lookahead == '_') ||
                ('a' <= lookahead && lookahead <= 'z'))
                ADVANCE(8);
            ACCEPT_TOKEN(sym_variable);
        case 9:
            if ((lookahead == '\t') ||
                (lookahead == '\n') ||
                (lookahead == '\r') ||
                (lookahead == ' '))
                SKIP(9);
            if (lookahead == '#')
                ADVANCE(2);
            if (lookahead == '(')
                ADVANCE(3);
            if ('0' <= lookahead && lookahead <= '9')
                ADVANCE(7);
            if (('A' <= lookahead && lookahead <= 'Z') ||
                ('a' <= lookahead && lookahead <= 'z'))
                ADVANCE(8);
            LEX_ERROR();
        case 10:
            if (lookahead == 0)
                ADVANCE(1);
            if ((lookahead == '\t') ||
                (lookahead == '\n') ||
                (lookahead == '\r') ||
                (lookahead == ' '))
                SKIP(10);
            if (lookahead == '#')
                ADVANCE(2);
            if (lookahead == ')')
                ADVANCE(4);
            if (lookahead == '*')
                ADVANCE(5);
            if (lookahead == '+')
                ADVANCE(6);
            LEX_ERROR();
        case 11:
            if (lookahead == 0)
                ADVANCE(1);
            if ((lookahead == '\t') ||
                (lookahead == '\n') ||
                (lookahead == '\r') ||
                (lookahead == ' '))
                SKIP(11);
            if (lookahead == '#')
                ADVANCE(2);
            if (lookahead == '*')
                ADVANCE(5);
            if (lookahead == '+')
                ADVANCE(6);
            LEX_ERROR();
        case 12:
            if ((lookahead == '\t') ||
                (lookahead == '\n') ||
                (lookahead == '\r') ||
                (lookahead == ' '))
                SKIP(12);
            if (lookahead == '#')
                ADVANCE(2);
            if (lookahead == ')')
                ADVANCE(4);
            if (lookahead == '*')
                ADVANCE(5);
            if (lookahead == '+')
                ADVANCE(6);
            LEX_ERROR();
        default:
            LEX_ERROR();
    }
}

static TSLexMode ts_lex_modes[STATE_COUNT] = {
    [0] = {.lex_state = 0},
    [1] = {.lex_state = 9},
    [2] = {.lex_state = 9},
    [3] = {.lex_state = 10},
    [4] = {.lex_state = 11},
    [5] = {.lex_state = 9},
    [6] = {.lex_state = 9},
    [7] = {.lex_state = 10},
    [8] = {.lex_state = 10},
    [9] = {.lex_state = 12},
    [10] = {.lex_state = 10},
    [11] = {.lex_state = 9},
    [12] = {.lex_state = 9},
    [13] = {.lex_state = 10},
    [14] = {.lex_state = 10},
    [15] = {.lex_state = 10},
};

static unsigned short ts_parse_table[STATE_COUNT][SYMBOL_COUNT] = {
    [0] = {
        [sym_expression] = STATE(13),
        [sym_sum] = STATE(3),
        [sym_product] = STATE(3),
        [ts_builtin_sym_end] = ACTIONS(1),
        [anon_sym_LPAREN] = ACTIONS(3),
        [anon_sym_RPAREN] = ACTIONS(5),
        [anon_sym_PLUS] = ACTIONS(7),
        [anon_sym_STAR] = ACTIONS(9),
        [sym_number] = ACTIONS(11),
        [sym_comment] = ACTIONS(13),
        [sym_variable] = ACTIONS(11),
    },
    [1] = {
        [sym_expression] = STATE(4),
        [sym_sum] = STATE(3),
        [sym_product] = STATE(3),
        [anon_sym_LPAREN] = ACTIONS(15),
        [sym_number] = ACTIONS(17),
        [sym_comment] = ACTIONS(13),
        [sym_variable] = ACTIONS(17),
    },
    [2] = {
        [sym_expression] = STATE(9),
        [sym_sum] = STATE(3),
        [sym_product] = STATE(3),
        [anon_sym_LPAREN] = ACTIONS(15),
        [sym_number] = ACTIONS(17),
        [sym_comment] = ACTIONS(13),
        [sym_variable] = ACTIONS(17),
    },
    [3] = {
        [ts_builtin_sym_end] = ACTIONS(19),
        [anon_sym_RPAREN] = ACTIONS(19),
        [anon_sym_PLUS] = ACTIONS(19),
        [anon_sym_STAR] = ACTIONS(19),
        [sym_comment] = ACTIONS(13),
    },
    [4] = {
        [ts_builtin_sym_end] = ACTIONS(21),
        [anon_sym_PLUS] = ACTIONS(23),
        [anon_sym_STAR] = ACTIONS(25),
        [sym_comment] = ACTIONS(13),
    },
    [5] = {
        [sym_expression] = STATE(8),
        [sym_sum] = STATE(3),
        [sym_product] = STATE(3),
        [anon_sym_LPAREN] = ACTIONS(15),
        [sym_number] = ACTIONS(17),
        [sym_comment] = ACTIONS(13),
        [sym_variable] = ACTIONS(17),
    },
    [6] = {
        [sym_expression] = STATE(7),
        [sym_sum] = STATE(3),
        [sym_product] = STATE(3),
        [anon_sym_LPAREN] = ACTIONS(15),
        [sym_number] = ACTIONS(17),
        [sym_comment] = ACTIONS(13),
        [sym_variable] = ACTIONS(17),
    },
    [7] = {
        [ts_builtin_sym_end] = ACTIONS(27),
        [anon_sym_RPAREN] = ACTIONS(27),
        [anon_sym_PLUS] = ACTIONS(27),
        [anon_sym_STAR] = ACTIONS(27),
        [sym_comment] = ACTIONS(13),
    },
    [8] = {
        [ts_builtin_sym_end] = ACTIONS(29),
        [anon_sym_RPAREN] = ACTIONS(29),
        [anon_sym_PLUS] = ACTIONS(29),
        [anon_sym_STAR] = ACTIONS(25),
        [sym_comment] = ACTIONS(13),
    },
    [9] = {
        [anon_sym_RPAREN] = ACTIONS(31),
        [anon_sym_PLUS] = ACTIONS(23),
        [anon_sym_STAR] = ACTIONS(25),
        [sym_comment] = ACTIONS(13),
    },
    [10] = {
        [ts_builtin_sym_end] = ACTIONS(33),
        [anon_sym_RPAREN] = ACTIONS(33),
        [anon_sym_PLUS] = ACTIONS(33),
        [anon_sym_STAR] = ACTIONS(33),
        [sym_comment] = ACTIONS(13),
    },
    [11] = {
        [sym_expression] = STATE(15),
        [sym_sum] = STATE(3),
        [sym_product] = STATE(3),
        [anon_sym_LPAREN] = ACTIONS(15),
        [sym_number] = ACTIONS(17),
        [sym_comment] = ACTIONS(13),
        [sym_variable] = ACTIONS(17),
    },
    [12] = {
        [sym_expression] = STATE(14),
        [sym_sum] = STATE(3),
        [sym_product] = STATE(3),
        [anon_sym_LPAREN] = ACTIONS(15),
        [sym_number] = ACTIONS(17),
        [sym_comment] = ACTIONS(13),
        [sym_variable] = ACTIONS(17),
    },
    [13] = {
        [ts_builtin_sym_end] = ACTIONS(35),
        [anon_sym_RPAREN] = ACTIONS(39),
        [anon_sym_PLUS] = ACTIONS(43),
        [anon_sym_STAR] = ACTIONS(47),
        [sym_comment] = ACTIONS(13),
    },
    [14] = {
        [ts_builtin_sym_end] = ACTIONS(27),
        [anon_sym_RPAREN] = ACTIONS(27),
        [anon_sym_PLUS] = ACTIONS(51),
        [anon_sym_STAR] = ACTIONS(54),
        [sym_comment] = ACTIONS(13),
    },
    [15] = {
        [ts_builtin_sym_end] = ACTIONS(29),
        [anon_sym_RPAREN] = ACTIONS(29),
        [anon_sym_PLUS] = ACTIONS(57),
        [anon_sym_STAR] = ACTIONS(60),
        [sym_comment] = ACTIONS(13),
    },
};

static TSParseActionEntry ts_parse_actions[] = {
    [0] = {.count = 0, .reusable = false, .depends_on_lookahead = false},
    [1] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, RECOVER(0),
    [3] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, RECOVER(2),
    [5] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, RECOVER(10),
    [7] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, RECOVER(11),
    [9] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, RECOVER(12),
    [11] = {.count = 1, .reusable = true, .depends_on_lookahead = true}, RECOVER(3),
    [13] = {.count = 1, .reusable = true, .depends_on_lookahead = true}, SHIFT_EXTRA(),
    [15] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, SHIFT(2),
    [17] = {.count = 1, .reusable = true, .depends_on_lookahead = true}, SHIFT(3),
    [19] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, REDUCE(sym_expression, 1),
    [21] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, ACCEPT_INPUT(),
    [23] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, SHIFT(5),
    [25] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, SHIFT(6),
    [27] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, REDUCE_FRAGILE(sym_product, 3),
    [29] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, REDUCE_FRAGILE(sym_sum, 3),
    [31] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, SHIFT(10),
    [33] = {.count = 1, .reusable = true, .depends_on_lookahead = false}, REDUCE(sym_expression, 3),
    [35] = {.count = 3, .reusable = true, .depends_on_lookahead = false}, ACCEPT_INPUT(), REDUCE_FRAGILE(sym_sum, 3), REDUCE_FRAGILE(sym_product, 3),
    [39] = {.count = 3, .reusable = true, .depends_on_lookahead = false}, REDUCE_FRAGILE(sym_sum, 3), REDUCE_FRAGILE(sym_product, 3), SHIFT(10),
    [43] = {.count = 3, .reusable = true, .depends_on_lookahead = false}, REDUCE_FRAGILE(sym_sum, 3), REDUCE_FRAGILE(sym_product, 3), SHIFT(11),
    [47] = {.count = 3, .reusable = true, .depends_on_lookahead = false}, REDUCE_FRAGILE(sym_sum, 3), REDUCE_FRAGILE(sym_product, 3), SHIFT(12),
    [51] = {.count = 2, .reusable = true, .depends_on_lookahead = false}, REDUCE_FRAGILE(sym_product, 3), SHIFT(11),
    [54] = {.count = 2, .reusable = true, .depends_on_lookahead = false}, REDUCE_FRAGILE(sym_product, 3), SHIFT(12),
    [57] = {.count = 2, .reusable = true, .depends_on_lookahead = false}, REDUCE_FRAGILE(sym_sum, 3), SHIFT(11),
    [60] = {.count = 2, .reusable = true, .depends_on_lookahead = false}, REDUCE_FRAGILE(sym_sum, 3), SHIFT(12),
};

const TSLanguage *tree_sitter_arithmetic() {
    GET_LANGUAGE();
}
