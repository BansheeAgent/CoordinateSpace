////////////////////////////////////////////////////////////////////////////////
/*
 * The global picture
 *  To transform the coordinates from one space to the next coordinate space
 *  we'll use several transformation matrices of which the most important are
 *  the model, view and projection matrix. Our vertex coordinates first start
 *  in local space as local coordinates and are then further processed to
 *  world coordinates, view coordinates, clip coordinates and eventually end
 *  up as screen coordinates.
 *
 *  1. Locat coordinates are the coordinates of your object relative to its
 *  local origin; ther're the coordinates your object begins in.
 *
 *  2. The next step is to transform the local coordinates to world-space
 *  coordinates which are coordinates in respect of a larger world. These
 *  coordinates are relative to some global origin of the world, together
 *  with many other objects also placed relative to this world's origin.
 *
 *  3. Next we transform the world coordinates to view-space coordinates in
 *  such a way that each coordinate is as seen from the camera or viewer's
 *  point of view.
 *
 *  4. After the coordinates are in view space we want to project them to
 *  clip coordinates. Clip coordinates are processed to the -1.0 and 1.0
 *  range and determine which vertices will end up on the screen. Projection
 *  to clp-space coordinates can add perspective if using perspective
 *  projection.
 *
 *  5. And lastly we transform the clip coordinates to screen coordinates in
 *  a process we call viewport transform that transforms the coordinates from
 *  -1.0 and 1.0 to the coordinate range defined bl glViewport. The resulting
 *  coordinates are then sent to the rasterizer to turn them into fragments.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Local Space
 *  Local space is the coordinate space that is local to your object, i.t.
 *  where your object begins in. Imagine that you've created your cube in
 *  modeling software package ( like Blender). The origin of your cube is
 *  probably at (0,0,0) even though your cube might end up at a different
 *  location in your final application. Probably all the models you've
 *  created all have (0,0,0) as their initial position. All the vertices of
 *  your model are therefore in local space: they are all local to your object.
 *
 *  The vertices of the container we've been using were specified as
 *  coordinates between -0.5 and 0.5 with 0.0 as its origin. These are local
 *  coordinates.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * World space
 *  If we would import all our objects directly in the application they would
 *  probably all be somewhere positioned inside each other at the world's
 *  origin of (0,0,0) which is not what we want. We want to define a position
 *  for each object to position them inside a larger world. The coordinates
 *  in world space are exactly what they sound like: the coordinates of all
 *  your vertices relative to a (game) world. This is the coordinate space
 *  where you want your objects transformed to in such a way that they're all
 *  scattered around the place (preferably in a realistic fashion). The
 *  coordinates of your object are transformed from local to world space;
 *  this is accomplished with the model matrix.
 *
 *  The model matrix is a transformation matrix that translates, scales
 *  and/or rotates your object to place it in the world at a
 *  location/orientation they belong to. Think of it as transforming it a but
 *  to the left on the y-axis so that it neatly fits with the neighboring
 *  houses. You could think of the matrix in the previous project to position
 *  the container all over the scene as a sort of model matrix as well; we
 *  transformed the local coordinates of the container to some different
 *  place in the scene/world.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * View Space
 *  The view space is what people usually refer to as the camera of OpenGL
 *  (it is sometimes also known as camera space or eye space). The view space
 *  is the result of transforming your world-space coordinates to coordinates
 *  that are in front of the user's view. The view space is the result of
 *  transforming your world space coordinates to coordinates that are in
 *  front of the user's view. The view space is thus the space as seen from
 *  the camera's point of view. This is usually accomplished with a
 *  combination of translations and rotations to translate/rotate the scene
 *  so that certain items are transformed to the front of the camera. These
 *  combined transformations are generally stored inside a view matrix that
 *  transforms world coordinates to view space.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Clip Space
 *  At the end of each vertex shader run, OpenGL expects the coordinates to
 *  be within a specific range and any coordinate that falls outside this
 *  range is clipped. Coordinates that are clipped are discarded, so the
 *  remaining coordinates will end up as fragments visible on your screen.
 *  This is also where clip space gets its name from.
 *
 *  Because specifying all the visible coordinates to be within the range -1
 *  .0 and 1.0 isn't really intuitive, we specify our own coordinate set to
 *  work in and convert thsoe back to NDC as OpenGL expects them.
 *
 *  To transform vertex coordinates from view to clip-space we define a so
 *  called projection matrix that specifies a range of coordinates e.g. -1000
 *  and 1000 in each dimension. The projection matrix then transforms
 *  coordinates within this specified range to normalized device coordinates
 *  (-1.0, 1.0). All coordinates outside this range will not be mapped
 *  between -1.0 and 1.0 and therefor be clipped. With this range we
 *  specified in the projection matrix, a coordinate of (1250, 500, 750)
 *  would not be visible, since the x coordinate is out of range and thus
 *  gets converted to a coordinate higher than 1.0 in NDC and is therefore
 *  clipped.
 *
 *  Note that if only a part of a primitive e.g. a triangle is outside the
 *  clipping volume OpenGL will reconrtuct the triangle as one or more
 *  triangles to fit inside the clipping range.
 *
 *  This viewing box a projection matrix creates is called a frustum and each
 *  coordinate that ends up inside this frustum will end up on the user's
 *  screen. The total process to convert coordinates within a specified range
 *  to NDC that can easily be mapped to 2D view-space coordinates is called
 *  projection since the projection matrix projects 3D coordinates to the
 *  easy-to-map-to-2D normalized device coordinates.
 *
 *  Once all the vertices are transformed to clip space a final operation
 *  called perspective division is performed where we divide the x, y, and z
 *  components of the position vectors by the vector's homogeneous w
 *  component; perspective division is what transforms the 4D clip space
 *  coordinates to 3D normalized device coordinates. This step is performed
 *  automatically at the end of the vertex shader step.
 *
 *  It is after this stage where the resulting coordinates are mapped to
 *  screen coordinates (using the settings of glViewport) and turned into
 *  fragments.
 *
 *  The projection matrix to transform view coordinates to clip coordinates
 *  usually takes two different forms, where each form defines its own unique
 *  frustum. We can either create an orthographic projection matrix or a
 *  perspective projection matrix.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Orthographic Projection
 *  An orthographic projection matrix defines a cube-like frustrum box that
 *  defines the clipping space where each vertex outside this box is clipped.
 *  When creating an othographic projection matrix we specify the width,
 *  height and length of the visible frustum. All the coordinates inside this
 *  frustum will end up within the NDC range after transformed by its matrix
 *  and thus won't be clipped. The frustum looks like a container
 *
 *  The frustum defines the visible coordinates and is specified by a width,
 *  a height and a near and far plane. Any coordinate in front of the near
 *  plane is clipped and the same applies to coordinates behind the far plane
 *  . The orthographic frustum directly maps all coordinates inside the
 *  frustum to normalized device coordinates without any special side effects
 *  since it won't touch the w component of the transformed vector; If the w
 *  component remains equal to 1.0 perspective division won't change the
 *  coordinates.
 *
 *  To create an orthographic projection matrix we make use of GLM's built-in
 *  function glm::ortho. The first two parameters specify the left and right
 *  coordinate of the frustum and the third and fourth parameter specify the
 *  bottom and top part of the frustum. With those 4 points we've defined the
 *  size of the near and far planes and the 5th and 6th coordinates between
 *  these x, y, and z range values to normalized device coordinates.
 *
 *  An orthographic projection matrix directly maps coordinates to the 2D
 *  plane that is your screen, but in reality a direct projection produces
 *  unrealistic results since the projection doesn't take perspective into
 *  account. That is something the perspective projection matrix fixes for us.
 */
////////////////////////////////////////////////////////////////////////////////
#if 0
  glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
#endif
////////////////////////////////////////////////////////////////////////////////
/*
 * Perspective Projection
 *  If you ever were to enjoy the graphics the real life has to offer you'll
 *  notice that objects that are farther away appear much smaller. This weird
 *  effect is something we call perspective. Perspective is especially
 *  noticeable when looking down the end of an infinite motorway or railway
 *
 *  the lines coincide at a far enough distance. This is exactly the effect
 *  perspective projection tries to mimic and it does so using a perspective
 *  projection matrix. The projection matrix maps a given frustum range to
 *  clip space, but also manipulates the w value of each vertex coordinate in
 *  such a way that the further away vertex coordinate is from the viewer,
 *  the higher this w component becomes. Once the coordinates are transformed
 *  to clip space they are in the range -w to w (anything outside this range
 *  is clipped). OpenGL requires that the visible coordinates fal between the
 *  range -1.0 and 1.0 as the final vertex shader output, thus once the
 *  coordinates are in clip space, perspective division is applied to the
 *  clip space coordinates:
 *
 *  Each component of the vertex coordinate is divided by its w component
 *  giving smaller vertex coordinates the further away a vertex is from the
 *  viewer, This is another reason why the w component is important, since it
 *  helps us with perspective projection. The resulting coordinates are then
 *  in normalized device space. If you're interested to out how the
 *  orthographic and perspective projection matrices are actually calculated
 *  look at this,
 *    https://www.songho.ca/opengl/gl_projectionmatrix.html
 *
 *  What glm::perspective does is again create a large frustum that defines
 *  the visible space, anything outside the frustum will not end up in the
 *  clip space volume and will thus become clipped. A perspective frustum can
 *  be visualized as a non-uniformly shaped box from where each coordinate
 *  inside this box will be mapped to apoint in clip space.
 *
 *  its first parameter defines the fov value, that stands for field of view
 *  and sets how large the viewspace is. For a realistic view it is usually
 *  set to 45 degrees, but for more doom-style results you could set to to a
 *  higher value. The second parameter sets the aspect ratio which calculated
 *  by dividing the viewport's width by its height. The third and fourth
 *  parameter set the near and far plane of the frustum. We usually set the
 *  near distance to 0.1 and the far distance to 100.0. All the vertices
 *  between the near and far plane and inside the frustum will be rendered.
 *  Whenever the near value of your perspective matrix is set too high( like
 *  10.0), OpenGL will clip all coordinates close to the camera(between 0.0
 *  and 10.0), which can give a visual result you maybe have seen before in
 *  videogames where you could see through certain objects when moving
 *  uncomfortably close to them.
 *
 *  When using oorhtographic projection, each of the vertex coordinates are
 *  directly mapped to clip space without any fancy perspective division (it
 *  still does perspective projection, objects farther away do not seem
 *  smaller wich produces a weird visual output. For this reason the
 *  orthographic projection is mainly used for 2D renderings and for some
 *  architectural or engineering applications where we'd rather not have
 *  vertices distorted by perspective. Application like Blender that are used
 *  for 3D modelling sometimes use orthographic projection for modelling,
 *  because it more accurately depicts each object's dimensions. Below you'll
 *  see a comparison of both projection methods in Blender
 */
////////////////////////////////////////////////////////////////////////////////

#if 0
  glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * Putting it all together
 *  We create a transformation matrix for each of the aforementioned steps:
 *  model, view and projection matrix. A vertex coordinate is then
 *  transformed to clip coordinates as follows:
 *
 *    V_clip = M_projection * M_view * M_model * V_local
 *
 *  Note that the order of matrix multiplication is reserved (remember that
 *  we need to read matrix multiplication from right to left). The resulting
 *  vertex should then be assigned to gl_position in the vertex shader and
 *  OpenGL will then automatically perform perspective division and clipping.
 *
 *  AND THEN?
 *    the output of the vertex shader requires the coordinates to be in
 *    clip-space which is what we just did with the transformation matrices.
 *    OpenGL the performs perspective division on the clip-space coordinates
 *    to transform them to normalized-device-coordinates to screen
 *    coordinates where each coordinate corresponds to a point on your screen
 *    (in our case a 800x600 screen). This process is called the viewport
 *    transform.
 *
 *  This is a difficult topic to understand so if you're still not exactly
 *  sure about what each space is used for you don't have to worry. Below
 *  you'll see how we can actually put these coordinate spaces to good use
 *  and enough examples will follow in the upcoming chapters.
 */
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/*
 * Going 3D
 *  Now that we know how to transform 3D coordinates to 2D coordinates we can
 *  start rendering real 3D objects instead of the lame 2D plane we've been
 *  showing so far.
 *
 *  To start drawing in 3D we'll first create a model matrix. The model
 *  matrix consists of translations, scaling and/or rotations we'd like to
 *  apply to transform all object's vertices to the global world space. Let's
 *  transform our plane a bit by rotating it on the x-axis so it looks like
 *  it's laying on the floor. The model matrix then looks like the code below
 *
 *  By multiplying the vertex coordinates with this model matrix we're
 *  transforming the vertex coordinates to world coordinates Our plane that
 *  is slightly on the floor thus represents the plane in the global world.
 *
 *  Next we need to create a view matrix. We want to move slightly backwards
 *  in the scene so the object becomes visible( when in world space we're
 *  located at the origin (0, 0, 0)). To move around the scene, Think about
 *  the following:
 *    + To move a camera backwards, is the same as moving the entire scene
 *    forward.
 *
 *   That is exactly what a view matrix does, we move the entire scene around
 *   inversed to where we want the camera to move because we want to move
 *   backwards and since OpenGL is a right-handed system we have to move in
 *   the positive z-axis. We do this by translating the scene towards the
 *   negative z axis. This gives the impression that we are moving backwards.
 */
////////////////////////////////////////////////////////////////////////////////

#if 0
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
#endif


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // build and compile our shader zprogram
  // ------------------------------------
  Shader ourShader("4.1.texturevs.txt", "4.1.texturefs.txt");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
          // positions          // colors           // texture coords
          0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
          0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
          -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
          -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
          0, 1, 3, // first triangle
          1, 2, 3  // second triangle
  };
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);


  // load and create a texture
  // -------------------------
  // bind textures one corresponding texture units
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);


  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians((float)glfwGetTime() * 50.0f),
            glm::vec3(0.0f, 0.0f, 1.0f));
    //first rotate a plane a little so that it looks like its laying down
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //next we need to create a view matrix
    glm::mat4 view = glm::mat4(1.0f);
        //note that we're translating the scene in the reverse direction of
        // where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    //The last thing we need is to define the projection matrix.
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,100.0f);

    //Now that we created the transformation matrices we should pass them to
    // our shaders. First let's declare the transformation matrices as
    // uniforms in the vertex shader and multiply them with the vertex
    // coordinates. please go to the vertex shader file.

    //After that bit we should come back here and send the matrices to the
    // shader. (this is usually done each frame since transformation matrices
    // tend to change a lot
    int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


    // render container
    ourShader.use();
    int uniformTime = glGetUniformLocation(ourShader.getProgram(), "time");
    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glUniform1f(uniformTime, glfwGetTime());


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}