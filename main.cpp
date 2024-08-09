/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inghwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 13:33:06 by inghwang          #+#    #+#             */
/*   Updated: 2024/07/08 13:33:08 by inghwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/Kq.hpp"

int logs = open("./log", O_WRONLY | O_CREAT | O_TRUNC, 0644);

int main(void)
{
    Kq  kq;

    //cgi실행할 fork
    std::ios::sync_with_stdio(false);
    while (1)
        kq.mainLoop();
    close(logs);
}
