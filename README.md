# Billiard Game based on Opengl

# 1 Basic Points

### 1 3D model

* Based on OpenGL，basic（Cube、Sphere、...）
  * **Balls 1-15、white ball**, radius: 0.006f in opengl 
  * **Table**：Surface size: 0.286f·0.140f

### 2 obj Loader

* load_obj class.
  * **Realize way：**

    Parse “v”（point coordinates）、“v_t”（texture）、“v_n”（normal）load into vertex。

    “f” store vertex information that makes up each face。

    Read mtl file to load the textures.


### 3 Scene roaming

* Operations：
  * ’up': camera shift up
  * 'down': camera shift down
  * 'left': camera shift left
  * 'right': camera shift right


#2 Awesome Points

### 1 Physical collision model

#### (1) Physics basis
* **Frictional resistance**  

  * Newton's second law: f = m * a  

  * Speed and acceleration: v_t = v_0 + a * t  

  * The ground friction is opposite to the direction of the moving object, hindering the forward movement of the object.


# 3 Dependencies

* freeglut
* glm
* glew
* assimp
* gli
* il

