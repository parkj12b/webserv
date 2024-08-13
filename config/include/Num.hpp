/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Num.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:49:13 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/13 16:37:28 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __NUM_HPP__
# define __NUM_HPP__

#include <string>
#include "Token.hpp"
#include "UtilTemplate.hpp"

using namespace std;

/**
 * @brief                       class for number token
 * @param   value               value of the number
*/
class Num : public Token {
public:
    const int value;
    string toString();
    Token *clone();
    Num(int v);
};

#endif