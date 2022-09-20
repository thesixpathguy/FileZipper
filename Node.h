//
// Internal Node Structure.
//

#ifndef HUFFMANCODING_NODE_H
#define HUFFMANCODING_NODE_H


class Node {
    char character;
    int frequency;

public:
    int getFrequency() const
    {
        return this->frequency;
    }

private:
    Node * left, * right;

public:
    Node(char character, int frequency)
    {
        this->left = this->right = nullptr;
        this->character = character;
        this->frequency = frequency;
    }

    Node(char character)
    {
        this->character=character;
        this->left = this->right = nullptr;
    }

    Node *getLeft() const
    {
        return this->left;
    }

    Node *getRight() const
    {
        return this->right;
    }

    void setLeft(Node *left)
    {
        this->left=left;
    }

    void setRight(Node *right)
    {
        this->right=right;
    }

    char getCharacter() const
    {
        return this->character;
    }

};


#endif //HUFFMANCODING_NODE_H
