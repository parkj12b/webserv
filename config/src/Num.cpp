/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Num.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:49:36 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/09 19:54:42 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Num.hpp"
#include "Tag.hpp"
#include "Token.hpp"
#include "UtilTemplate.cpp"

using namespace std;

string Num::toString() {
    return ::toString(value);
}

Num::Num(int v) : Token(Tag::NUM), value(v) {}