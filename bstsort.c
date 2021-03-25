/*
  Name: Karanveer Sandhu
  PID: 6096890
  I hearby declare that this source code is original
  and not anyone else's work.
*/

/*
  Program Description:
  This program reads user input from the screen or an
  input file and prints out the lines that are read.
  Then, those lines are sorted, case-sensitive or not using
  the cflag, into a binary tree. Lines that are identical are 
  stored in the same node and that node will have an increased 
  count. Then, the node will print each line and
  the count of each line in alphabetical order. 
  Finally, the nodes allocated memory are freed at the end of
  the program.
  Usage: bstsort [-c] [-o output_file_name] [input_file_name]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A node of the binary tree.
typedef struct Node
{
  char string[101]; // the contents of said node
  struct Node* left;       // left child node pointer
  struct Node* right;      // right child node pointer
  int count;        // # of string incidents.
} Node; // end of Node struct

// Adds a node/subtree to an already existing tree.
void addNode(Node* toTree, Node* fromNode, char word[])
{
  // BASE CASE: if tree is empty, for the root node.
  if (toTree->count == 0)
  {
    free(fromNode); // Didn't need to malloc that fromNode anyway.
    toTree->count = 1;   // that nodes has 1 instance of the word.
    strcpy(toTree->string, word); // that node has that word too.
    return;
  }
  
  // If word is less than the current tree's word
  if (strCompareTo(word, toTree->string) < 0)
  {
    // If the left tree has nothing.
    if (toTree->left == NULL)
    {
      toTree->left = fromNode; // that left Node gets the new node
      strcpy(fromNode->string, word); // said Node gets the word
      fromNode->count = 1; // said node has one instance of variable
      return;
    }
    // otherwise
    addNode(toTree->left, fromNode, word); // just add the node to the left subtree
  }
  else if (strCompareTo(word, toTree->string) > 0)
  {
    // If the right tree has nothing.
    if (toTree->right == NULL)
    {
      toTree->right = fromNode; // that right Node gets the new node
      strcpy(fromNode->string, word); // said Node gets the word
      fromNode->count = 1; // said node has one instance of variable
      return;
    }
    // otherwise
    addNode(toTree->right, fromNode, word); // just add the node to the right subtree
  }
  else // the words are identical
  {
    toTree->count++; // That tree has one more instance of the string
    free(fromNode); // Didn't need to malloc that new Node
  } // end of if-else chain
    
} // end of addNode() method

// traverses the binary tree in order and prints out the
// contents line by line. For printing to screen.
void inOrderTraversal(Node* tree)
{
  // BASE CASE: if the subtree/node is not there
  if (tree == NULL)
    return; // go back to previous function call
  
  // Call the inOrderTraversal of left subtree
  inOrderTraversal(tree->left);

  // Print contents of current Node and its number
  // of occurances..
  printf("%d %s\n", tree->count, tree->string);
  
  // Call the inOrderTraversal of the right subtree.
  inOrderTraversal(tree->right);
} // end of inOrderTraversal() function

// traverses the binary tree in order and prints out the
// contents line by line. For printing to outputFile.
void inOrderTraversalF(Node* tree, FILE* outFile)
{
  // BASE CASE: if the subtree/node is not there
  if (tree == NULL)
    return; // go back to previous function call
  
  // Call the inOrderTraversal of left subtree
  inOrderTraversalF(tree->left, outFile);

  // Print contents of current Node and its number
  // of occurances to the file
  fprintf(outFile,"%d %s\n", tree->count, tree->string);
  
  // Call the inOrderTraversal of the right subtree.
  inOrderTraversalF(tree->right, outFile);
} // end of inOrderTraversal() function

// Deallocates the memory of the tree by postOrderTraversal
void freeTheTree(Node* tree)
{
  // If that subtree does not exist.
  if (tree == NULL)
    return; // do nothing

  // First free the left subtree
  freeTheTree(tree->left);

  // Then, free the right subtree
  freeTheTree(tree->right);

  // Finally, free the "root" tree itself
  free(tree);
}

// Makes the string at address string all to Caps.
void allToCaps(char* string , int size)
{
  char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
  int i;
  int j;
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < 27; j++)
    {
      // if that char is an uppercase char.
      if (*(string + i) == alphabet[j])
      {
        // Make lower case char upper.
        *(string + i) -= 32;
      } // end of if.
    } // end of alphabet's for loop
  } // end of string's for loop
} // end of allToCaps() function.

// Compare strings to see which comes first
// in a dictionary
int strCompareTo(char a[], char b[])
{
  int alength = strlen(a);
  int blength = strlen(b);
  int i;
  for (i = 0; i < alength || i < blength; i++)
  {
    // if A ends before B does, or A's char is smaller
    if (a[i] == '\0' || a[i] < b[i])
    {
      // a was smaller
      return -1;
    }

    // B ends before A does or B's char is smaller
    if (b[i] == '\0' || b[i] < a[i])
    {
      // a was bigger
      return 1;
    }
  } // end of for loop

  // If the code manages to reach this point
  // Both the of the strings are identical.
  return 0;
} // End of strCompareTo() function.

// Removes the \n at the end of the string.
void removeNewline(char* string, int length)
{
  if (*(string + length - 1) == '\n')
    *(string + length - 1) = '\0';
  return;
}

int main(int argc, char **argv)
{
  // These are the getopt variables
  extern char *optarg;
  extern int optind;
  int c;
  
  // The flags of command line;
  int cflag = 0;
  int oflag = 0;

  // The input file name
  char inputName[101];

  // Determines whether there was an input file or not 
  int isInputFile = 0;

  // Output filename
  char outputName[101];

  // Whether the command line is an error
  int error = 0;

  // The root tree
  Node* root = NULL;

  // The input file
  FILE* inFile = NULL;

  // The output file
  FILE* outFile = NULL;

  while ((c = getopt(argc, argv, "co:")) != -1)
    switch (c)
    {
      case 'c':
        cflag = 1;
        break;
      case 'o':
        oflag = 1;
        strcpy(outputName, optarg);
        break;
      case '?':
        error = 1;
        break;
    }

  // if the commandline does not have at least one argument
  if ((optind + 2) > argc)
  {
    // Use stdin as the input;
  }
  else
  {
    // Use inputfile name as the input
    strcpy(inputName, argv[argc - 1]);
    isInputFile = 1;
  }

  // if oflag, try to open file.
  if (oflag)
  {
    outFile = fopen(outputName, "w");
      if (outFile == NULL)
      {
        printf("Could not open file %s.\n", outputName);
        printf("Usage: bstsort [-c] [-o output_file_name] [input_file_name]\n\n");
        return -1;
      }
  }

  // Allot space for root node.
  root = (Node*)malloc(sizeof(Node));
  
  // If there is an inputfile
  if (isInputFile)
  {
    // try to open the infile
    inFile = fopen(inputName, "r");
    if (inFile == NULL)
    {
      printf("Could not open file %s.\n\n", inputName);
      printf("Usage: bstsort [-c] [-o output_file_name] [input_file_name]\n\n");
      return -1;
    }
    
    // The char pointer that points to the current line
    // in the inputfile
    char currentLine[101];
    
    // While the inputfile can still be read
    while (fgets(currentLine, 101, inFile) != NULL)
    {
      // We have a new node to insert.
      Node* newNode = NULL;

      // remove that \n at the end, if there is one.
      removeNewline(&currentLine, strlen(currentLine));

      // If its not case sensitive
      if (!cflag)
        allToCaps(&currentLine, strlen(currentLine)); // put the string in all lowercase
      
      // Print out the line(s).
      // Depends on whether we have an outputfile
      if (oflag)
      {
        fprintf(outFile, "%s\n", currentLine);
      }
      else // we have to print to the screen 
      {
        printf("%s\n", currentLine);
      }
      
      // Add each currentLine to a new Node
      newNode = (Node*)malloc(sizeof(Node));
      addNode(root, newNode, currentLine);
      
    } // end of while loop reading the inputfile

    // Print a newline for neatness.
    if (oflag)
    {
      fprintf(outFile, "\n");
    }
    else
    {
      printf("\n"); 
    }

    // Then do the inOrderTraversal
    // which one depends on -o flag
    if (oflag)
    {
      inOrderTraversalF(root, outFile);
    } else
    {
      inOrderTraversal(root);
    }
    
    // Finally, close the input file
    fclose(inFile);

    // If there's an outputfile
    if (oflag)
      fclose(outFile); // close that as well.
  }
  else // there is no inputfile
  {
    // The char pointer that points to the current line
    // in the inputfile
    char currentLine[101];

    printf("Enter your lines below. Enter a blank line to stop.\n");
    
    // Determines whether the user inputted a blank line.
    int newline = 0;
    
    // While the inputfile can still be read and user didn't put a blank line
    while (!newline && fgets(currentLine, 101, stdin) != NULL)
    {
      // We have a new node to insert.
      Node* newNode = NULL;

      if (strCompareTo(currentLine, "\n") == 0)
      {
        newline = 1;
      }
      else
      {
        // remove that \n at the end, if there is one.
        removeNewline(&currentLine, strlen(currentLine));

        // If its not case sensitive
        if (!cflag)
          allToCaps(&currentLine, strlen(currentLine)); // put the string in all lowercase
      
        // Print out the line(s).
        // Depends on whether we have an outputfile
        if (oflag)
        {
          fprintf(outFile,"%s\n", currentLine);
        }
      
        // Add each currentLine to a new Node
        newNode = (Node*)malloc(sizeof(Node));
        addNode(root, newNode, currentLine); 
      }
    } // end of while loop reading the inputfile
    
    // Print a newline for neatness.
    if (oflag)
    {
      fprintf(outFile, "\n");
    }
    else
    {
      printf("\n"); 
    }

    // Then do the inOrderTraversal
    // which one depends on -o flag
    if (oflag)
    {
      inOrderTraversalF(root, outFile);
    } else
    {
      inOrderTraversal(root);
    }

    // If there's an outputfile
    if (oflag)
      fclose(outFile); // close that
  }

  // Lastly, we must free the trees
  free(root);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~Thank you for grading my program.~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
