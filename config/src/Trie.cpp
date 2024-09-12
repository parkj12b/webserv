/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Trie.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsepar <minsepar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:04:44 by minsepar          #+#    #+#             */
/*   Updated: 2024/08/17 01:01:51 by minsepar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
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

Trie::TrieNode::TrieNode(const TrieNode& other) {
    deepCopy(&other);
}

Trie::TrieNode& Trie::TrieNode::operator=(const TrieNode& other) {
    if (this != &other) {
        // Clean up current state
        for (map<char, TrieNode*>::iterator it = children.begin(); it != children.end(); ++it) {
            delete it->second;
        }
        children.clear();
        // Deep copy from other
        deepCopy(&other);
    }
    return *this;
}

void Trie::TrieNode::deepCopy(const TrieNode* other) {
    locationPath = other->locationPath;
    for (map<char, TrieNode*>::const_iterator it = other->children.begin();
        it != other->children.end(); ++it) {
        children[it->first] = new TrieNode(*(it->second));
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
        ? ""
        : longestMatch;
}

Trie::Trie(const Trie& other) : root(NULL) {
    deepCopy(&other);
}

Trie& Trie::operator=(const Trie& other) {
    if (this != &other) {
        deleteTrie(root);
        root = NULL;
        deepCopy(&other);
    }
    return *this;
}

void Trie::deepCopy(const Trie* other) {
    if (other->root) {
        root = new TrieNode(*other->root);
    }
}

void Trie::deleteTrie(TrieNode* node) {
    if (node) {
        for (map<char, TrieNode*>::iterator it = node->children.begin();
            it != node->children.end(); ++it) {
            deleteTrie(it->second);
        }
        delete node;
    }
}

