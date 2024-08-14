/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Trie.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:00:45 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/14 14:21:55 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __TRIE_HPP__
# define __TRIE_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

// Trie Class Definition
class Trie {
public:
    // Trie Node Definition
    struct TrieNode {
        map<char, TrieNode*> children;
        string locationPath; // Store the location path if this node marks the end of a path

        TrieNode();
        ~TrieNode();
    };

    // Insert a path into the trie
    void insert(const string& path);

    // Find the longest matching prefix for a given URL
    string find(const std::string& url) const;
    Trie();
    ~Trie();

private:
    TrieNode* root;
};

#endif