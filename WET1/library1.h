/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* File Name : library1.h                                                */
/*                                                                          */
/****************************************************************************/

#ifndef _234218_WET1_DICT
#define _234218_WET1_DICT

#ifdef __cplusplus
extern "C" {
#endif

/* Return Values
 * ----------------------------------- */
typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    ALLOCATION_ERROR = -2,
    INVALID_INPUT = -3
} StatusType;

/* Required Interface for the Data Structure
 * -----------------------------------------*/

/* Description:   Initiates the data structure.
 * Input:         None.
 * Output:        DS - A pointer to the data structure.
 * Return Values: A pointer to a new instance of the data structure - as a void* pointer.
 */
void *Init();

/* Description:   Adds a new item to the data structure.
 * Input:         DS - A pointer to the data structure.
 *                key - The key to add.
 *                value - A pointer to the value to add.
 				  node - A pointer to a place which will hold a pointer to the new node in the DS.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                FAILURE - If the item is already in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType Add(void *DS, int key, void* value, void** node);

/* Description:   Finds an item in the data structure.
 * Input:         DS - A pointer to the data structure.
 *                key - The item to find.
 * Output:        value - a pointer to a variable which will contain the value of the element.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                FAILURE - If the item does not exist in the DS.
 *                SUCCESS - If the item is found in the DS.
 */
StatusType Find(void *DS, int key, void** value);

/* Description:   Delete an item from the data structure.
 * Input:         DS - A pointer to the data structure.
 *                key - The item to delete.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                FAILURE - If the item does not exist in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType Delete(void *DS, int key);

/* Description:   Delete an item from the data structure.
 * Input:         DS - A pointer to the data structure.
 *                p - A pointer to the node to delete.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                FAILURE - If the item does not exist in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType DeleteByPointer(void *DS, void* p);

/* Description:   Returns the current number of items in the data structure.
 * Input:         DS - A pointer to the data structure.
 * Output:        n - A pointer to a variable that should be updated to the number of items in the data structure.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL.
 *                SUCCESS - Otherwise.
 */
StatusType Size(void *DS, int *n);

/* Description:   Quits and deletes the database.
 *                DS should be set to NULL.
 * Input:         DS - A pointer to the data structure.
 * Output:        None.
 * Return Values: None.
 */
void Quit(void** DS);

#ifdef __cplusplus
}
#endif

#endif    /*_234218_WET1_ */
