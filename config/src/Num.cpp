/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Num.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:49:36 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 20:38:08 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Num.hpp"
#include "Tag.hpp"

using namespace std;

string Num::toString() {
    return ::toString(value);
}

Token *Num::clone() {
    return new Num(dynamic_cast<Num *>(this)->value);
}

Num::Num(int v) : Token(Tag::NUM), value(v) {}