/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tag.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 19:29:30 by minsepar          #+#    #+#             */
/*   Updated: 2024/09/06 20:47:51 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __TAG_HPP__
# define __TAG_HPP__

/**
 * @brief                  class used for storing tag, different tokens
 */
class Tag {
public:
    enum Tags {
        ID = 256,
        IPV4 = 257,
        NUM = 258,
        DIRECTIVE = 259,
        CONTEXT = 260,
        MASK = 261,
        HTTPCODE = 262,
        BYTES = 263,
        TIME = 264,
        METHOD = 265,
        SYMBOL = 266,
        END = -1
    };
};

#endif