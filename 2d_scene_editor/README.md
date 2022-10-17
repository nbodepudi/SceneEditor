# 2D Triangle Editor and Rasterization

#### Steps followed: (From the base cpp code given)
- Initialized a 4 total VBO's, 2 for storing vertex positions, color and other 2 for storing line points positions and there colors
- Updated the shader code to match my current structure.

### Task1.1: Triangle soup editor
#### Insertion mode - When key i is pressed
Images showing inserting triangles 

![name](Images/Img19.png)
![name](Images/Img18.png)
![name](Images/Img17.png)
![name](Images/Img16.png)

#### Translation mode - when key O is pressed
I have took screenshots after coloring vertices.

Image showing when a triangle is selected

![name](Images/Img14.png)

Image when the selected triangle is moved.

![name](Images/Img13.png)

Image when the triangle beside the selected triangle is moved.
![name](Images/Img12.png)

#### Deletion mode - when key p is pressed
Image when one of the small triangle inserted is deleted

![name](Images/Img11.png)

### Task1.2: Rotation and scale
Image when selected triangle is rotated clockwise.


![name](Images/Img10.png)

Image when selected triangle rotated counter clockwise.

![name](Images/Img9.png)

Image when selected triangle is scaled up.

![name](Images/Img8.png)

Image when selected triangle is scaled down.

![name](Images/Img7.png)


### Task1.3: Vertex Color
when a mouse click is done vertex near the click becomes dark greyish as shown in the vertices of small triangles
By clicking number from 1 to 9 different colors can be generated for vertices.

![name](Images/Img15.png)


### Task1.4: View control
#### W, A, S, D keys
Moving the screen in all four directions.

![name](Images/Img6.png)
![name](Images/Img5.png)
![name](Images/Img4.png)
![name](Images/Img3.png)

#### +, - keys
Image when view is zoomed in and zoomed out.

![name](Images/Img2.png)
![name](Images/Img1.png)

### Task1.5: Key Framing
- I only implemented key framing and animations only in translation.
- Here is the gif

![name](Images/key_frame.gif)

### Task2
#### Steps followed
- Implemented all the transformations  in vertex shader
- I am passing a transformation matrix, which takes care of rotation, scale of the triangle in the shader without changing the vertices
- View_matrix * Transformation_matrix * vec4(position, 0.0, 1.0). This generates the triangle after applying transformations
- View_matrix is what used in view control.

