# C_Assignment1
## International Lecturing Olympics Scoring System

### Overview
This program processes lecturer class scores and calculates per-class and per-lecturer adjusted average scores. The program then ranks lecturers based on their average scores and determines the top three medalists (gold, silver, and bronze).

### Features
- Use of loops, if statements, functions, and arrays
- Implementation of score aggregation and sorting algorithms

### Task Description
- Input consists of multiple lines with a lecturer's identifier followed by student scores.
- For each class, discard the two lowest scores, sum the remaining, and compute the adjusted average.
- For each lecturer, discard the two lowest per-class scores, sum the rest, and compute the overall average.
- Rank lecturers by their overall average score, determining the gold, silver, and bronze winners.

### Requirements
- Lecturer identifier (char 'A' to 'Z')
- Up to 50 scores per class, between 0.0 and 10.0
- Up to 500 lectures in total
- Discard classes with fewer than three scores
- Compute adjusted averages by discarding two lowest scores for each class and lecturer

### Stages
1. **Reading the Data**: Parse the input, filter invalid classes, and compute per-class adjusted scores.
2. **Aggregate by Lecturer**: Compute per-lecturer average scores.
3. **Rank the Lecturers**: Sort and determine medalists, handling ties in scores.

### Constraints
- At most 26 lecturers ('A' to 'Z')
- At most 50 scores per class
- At most 500 lectures in total

## Technologies  
- C  

## Contributors
- Ryan Huang
- University of Melbourne COMP20005 Teaching Team (Skeleton Code Provider)

## License
This project is for academic purposes under the University of Melbourne's COMP20005 course.
