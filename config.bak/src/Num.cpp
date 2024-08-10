/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Num.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:49:36 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 15:29:00 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Num.hpp"
#include "Tag.hpp"

using namespace std;

string Num::toString() {
    return ::toString(value);
}

Num::Num(int v) : Token(Tag::NUM), value(v) {}