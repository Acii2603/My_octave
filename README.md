# Octave Simulator

**Author**: Alexandru-Andrei Ionita  
**Year**: 2022-2023  

## Source File: `my_octave.c`

### Program Overview

This program simulates basic matrix operations, reading commands through the `x` variable until the letter **Q** is entered. Upon receiving input, the program enters a **switch statement** to handle various matrix-related operations. Below is a breakdown of the functionality, covering 11 different cases:

1. **L - Add Matrix**  
   This command adds a new matrix to the array. Memory is allocated dynamically, with defensive programming checks to handle errors during allocation.
   
2. **D - Show Dimensions**  
   Displays the dimensions of a specified matrix by accessing the `nm` matrix at the given index.

3. **P - Print Matrix**  
   Prints the contents of a specified matrix to the output.

4. **C - Crop Matrix**  
   Crops a matrix based on user input for the specific rows and columns to retain.

5. **M - Multiply Matrices**  
   Multiplies two given matrices and stores the result in the designated location.

6. **O - Sort Matrices**  
   Sorts all matrices based on the sum of their elements, in ascending order.

7. **T - Transpose Matrix**  
   Transposes the specified matrix in place.

8. **R - Raise Matrix to Power**  
   Raises a given matrix to the specified power.

9. **F - Free Matrix**  
   Frees the memory allocated for a specific matrix.

10. **Q - Quit and Free Resources**  
    Frees all allocated memory and gracefully exits the program.

11. **Default Case**  
    If an unrecognized command is entered, the program displays an appropriate error message.

### Program Flow
The program continues to execute, processing each command entered by the user until the **Q** command is encountered, which cleans up all resources and terminates execution.

---
**Note**: This program ensures robust memory management, with defensive programming techniques implemented to handle potential errors and edge cases throughout the operations.
