#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

#define Key vector<enc_node>

using namespace std;

struct enc_node
{
    int x = 0;
    int y = 0;
};

string encrypt(string message, Key key)
{
    for (int i = 0; i < key.size(); i++)
    {
        char char_x = message[key[i].x];
        char char_y = message[key[i].y];
        message[key[i].x] = char_y;
        message[key[i].y] = char_x;
    }
    return message;
}

string decrypt(string message, Key key)
{
    reverse(key.begin(), key.end());
    for (int i = 0; i < key.size(); i++)
    {
        char char_x = message[key[i].x];
        char char_y = message[key[i].y];
        message[key[i].x] = char_y;
        message[key[i].y] = char_x;
    }
    return message;
}

Key generate_key(int enc_chunk_size, int secureLevel)
{
    Key key;

    for (int i = 0; i < secureLevel; i++)
    {
        enc_node n;
        n.x = rand() % enc_chunk_size;
        n.y = rand() % enc_chunk_size;
        key.push_back(n);
    }

    return key;
}

vector<string> splitInChunks(string message, int chunk_size)
{
    vector<string> chunks;
    int i = 0;
    while (i < message.length())
    {
        chunks.push_back(message.substr(i, chunk_size));
        i += chunk_size;
    }
    return chunks;
}

int writeKeyInFile(string filename, Key key)
{
    ofstream outputFile(filename);
    // Check if the file opened successfully
    if (!outputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    for (auto key_node : key)
    {
        outputFile << key_node.x << " " << key_node.y << endl;
    }

    outputFile.close();

    return 0;
}

vector<string> splitString(const string &s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

Key getKeyFromFile(string fileName)
{
    ifstream inputFile(fileName);
    Key out;

    // Check if the file opened successfully
    if (!inputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        exit(1);
    }

    while (!inputFile.eof())
    {
        string line;
        getline(inputFile, line);

        if (line.empty())
            return out;

        auto parts = splitString(line, ' ');
        enc_node node;

        node.x = stoi(parts[0]);
        node.y = stoi(parts[1]);

        out.push_back(node);
    }

    return out;
}

int encryptFile(string inputFilePath, string outputFilePath, int chunksize, Key key)
{
    ifstream inputFile(inputFilePath);
    ofstream outputFile(outputFilePath);

    // Check if the file opened successfully
    if (!inputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return 1;
    }
    // Check if the file opened successfully
    if (!outputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    while (!inputFile.eof())
    {
        char chunk[chunksize];
        inputFile.read(chunk, chunksize);
        string encryptedChunk = encrypt(chunk, key);
        outputFile << encryptedChunk;
    }

    return 0;
}

int decryptFile(string inputFilePath, string outputFilePath, int chunksize, Key key)
{
    ifstream inputFile(inputFilePath);
    ofstream outputFile(outputFilePath);

    // Check if the file opened successfully
    if (!inputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return 1;
    }
    // Check if the file opened successfully
    if (!outputFile.is_open())
    {
        cerr << "Error opening the file!" << endl;
        return 1;
    }

    while (!inputFile.eof())
    {
        char chunk[chunksize];
        inputFile.read(chunk, chunksize);
        string decryptedChunk = decrypt(chunk, key);
        outputFile << decryptedChunk;
    }

    return 0;
}

void test()
{
    cout << "MWENCRYPTION V1.0.0 TEST : Hello World" << endl;
    string input = "Hello World";
    Key key = generate_key(input.size(), 100);

    string encrypted = encrypt(input, key);
    string decrypted = decrypt(encrypted, key);

    cout << "encrypted:\n";
    cout << encrypted << endl;
    cout << "decrypted:\n";
    cout << decrypted << endl;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        test();
        return 0;
    }

    if (argc < 4)
    {
        cerr << "!min 3 arguments required" << endl;
        return 1;
    }

    string function = argv[1];

    if (function == "enc" && argc == 6)
    {
        string inputFilePath = argv[2];
        string outputFilePath = argv[3];
        int chunksize = stoi(argv[4]);
        auto key = getKeyFromFile(argv[5]);
        encryptFile(inputFilePath, outputFilePath, chunksize, key);
    }
    else if (function == "dec" && argc == 6)
    {
        string inputFilePath = argv[2];
        string outputFilePath = argv[3];
        int chunksize = stoi(argv[4]);
        auto key = getKeyFromFile(argv[5]);
        encryptFile(inputFilePath, outputFilePath, chunksize, key);
    }
    else if (function == "generate-key" && argc == 5)
    {
        Key key = generate_key(atoi(argv[2]), atoi(argv[3]));
        return writeKeyInFile(argv[4], key);
    }
    else
    {
        cerr << "argument error" << endl;
        return 1;
    }

    return 0;
}