/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Num.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:49:13 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/10 20:36:53 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __NUM_HPP__
# define __NUM_HPP__

#include <string>
#include "Token.hpp"
#include "UtilTemplate.hpp"

using namespace std;

class Num : public Token {
public:
    const int value;
    Num(int v);
    string toString();
    Token *clone();
};


#endif