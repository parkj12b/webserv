/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Env.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:29:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/01 14:56:25 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __ENV_HPP__
# define __ENV_HPP__

#include <map>
#include <string>
#include <lexer.hpp>

class Env {
private:
    map<Token, Id> table;
protected:
    Env &prev;
public:
    Env(Env &n) : prev(n) {}
    void put(Token w, Id i) { table[w] = i; }
    Id get(Token w) {
        for (Env e = this; e != NULL; e = e.prev) {
            Id found = e.table[w];
            if (found != NULL) return found;
        }
        return NULL;
    }
};

class Type : public Word {
public:
    int width;
    Type(string s, int tag, int w) : Word(s, tag), width(w) {}
    bool operator==(const Type &t) 
        { return lexeme == t.lexeme && tag == t.tag && width == t.width; }
    const static Type
        Int, Float, Char, Bool;
    static bool numeric(Type p) {
        return p == Type::Char || p == Type::Int || p == Type::Float;
    }
    static Type *max(Type p1, Type p2) {
        if (!numeric(p1) || !numeric(p2)) return NULL;
        else if (p1 == Type::Float || p2 == Type::Float) return new Type(Type::Float);
        else if (p1 == Type::Int || p2 == Type::Int) return new Type(Type::Int);
        else return new Type(Type::Char);
    }
};

const Type Type::Int = Type("int", Tag::BASIC, 4);
const Type Type::Float = Type("float", Tag::BASIC, 8);
const Type Type::Char = Type("char", Tag::BASIC, 1);
const Type Type::Bool = Type("bool", Tag::BASIC, 1);

class Array : public Type {
public:
    Type of;
    int size = 1;
    Array(int sz, Type p) : Type("[]", Tag::INDEX, sz * p.width), size(sz), of(p) {}
    string toString() { return string("[") + to_string(size) + "] " + of.toString(); }
};

class Node {
public:
    int lexline = 0;
    Node() { lexline = Lexer::line; }
    void error(string s) { throw runtime_error("near line " + to_string(lexline) + ": " + s); }
    
};

#endif