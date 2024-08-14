/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Trie.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:04:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/14 15:04:01 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <stdexcept>
#include "Trie.hpp"

using namespace std;

Trie::TrieNode::TrieNode() : locationPath("") {}

Trie::TrieNode::~TrieNode()
{
    // Clean up children nodes
    for (std::map<char, TrieNode*>::iterator it = children.begin(); it != children.end(); ++it) {
        delete it->second;
    }
}

Trie::Trie() : root(new TrieNode()) {}

Trie::~Trie()
{
    delete root;
}

void Trie::insert(const string& path)
{
    TrieNode* node = root;
        for (std::string::const_iterator it = path.begin(); it != path.end(); ++it) {
            char c = *it;
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->locationPath = path;
}

string Trie::find(const std::string& url) const
{
    TrieNode* node = root;
    std::string longestMatch;
    
    for (std::string::const_iterator it = url.begin(); it != url.end(); ++it) {
        char c = *it;
        if (node->children.find(c) == node->children.end()) {
            break;
        }
        node = node->children[c];
        if (!node->locationPath.empty()) {
            longestMatch = node->locationPath;
        }
    }
    
    return longestMatch.empty()
        ? throw runtime_error("no matching location found")
        : longestMatch;
}
