#include "ADTRecTree_utils.h"
#include "math.h"



RecTree rectree_get_subtree(RecTree tree, int pos){

    pos++;//turn it to 1-based
    int size=1;
    int posHeight=pos;

    if(tree==REC_TREE_EMPTY){
        return REC_TREE_EMPTY;
    }
    
    if (pos==1){
        return tree;
    }
    while(posHeight>1){     // find the height of the tree
            posHeight=posHeight/2;
            size++;
        }

    int magicNumber= pow(2,size-1); //max children nodes in the height of pos
    int normalNumber = pos % magicNumber;   //an index for nodes in height of pos



    if(normalNumber<magicNumber/2){ //check if its in the left side of root or right
        return rectree_get_subtree(rectree_left(tree),normalNumber+magicNumber/2-1);//-1 because of 0-based 
    }

    else{
        return rectree_get_subtree(rectree_right(tree),(normalNumber-1));//-1 because 0-based

    }
}       
    
      






RecTree rectree_replace_subtree(RecTree tree, int pos, RecTree subtree){
    RecTree new_tree ;
    pos++;
    int size = 1;
    int posHeight = pos;
    if (pos == 1 )
        return subtree;
    while(posHeight>1){     // find the height of the tree
            posHeight=posHeight/2;
            size++;
        }

    int magicNumber= pow(2,size-1); //max children nodes in the height of pos
    int normalNumber = pos % magicNumber;   //an index for nodes in height of pos


    if(normalNumber<magicNumber/2){ //check if its in the left side of root or right
        // new_tree = rectree_create(rectree_value(rectree_get_subtree(tree,pos/2-1)),subtree,rectree_get_subtree(tree,pos));
        // return rectree_replace_subtree( tree,  pos/2-1, new_tree);
        new_tree = rectree_create(rectree_value(tree),rectree_replace_subtree(rectree_left(tree),normalNumber+magicNumber/2-1,subtree),rectree_right(tree));
        
    }else{
        // new_tree = rectree_create(rectree_value(rectree_get_subtree(tree,pos/2-1)),rectree_get_subtree(tree,pos),subtree);
        // return rectree_replace_subtree(tree,  pos/2-1, new_tree);
        new_tree = rectree_create(rectree_value(tree),rectree_left(tree),rectree_replace_subtree(rectree_right(tree),normalNumber-1,subtree));
    }
    return new_tree;
    

}