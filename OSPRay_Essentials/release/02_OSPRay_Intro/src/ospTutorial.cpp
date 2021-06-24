

// system includes
#include <alloca.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

// ospray and math includes
#include "ospray/ospray_util.h"
#include "rkcommon/math/rkmath.h"
#include "rkcommon/math/vec.h"

// includes to STB for writing out a PNG image
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

using namespace rkcommon::math;

// ################################################################################
// A simple function that uses the STB library to write out the framebuffer in
// PNG format.

void writePNG(const char *fileName, const vec2i &size, const uint32_t *pixel) {
  constexpr int nChannels{4};
  const int stride{nChannels * size.x};
  stbi_write_png(fileName, size.x, size.y, nChannels, pixel, stride);
}

// ################################################################################
// The entry point into the program that exercises the API

int main(int argc, const char **argv) {

  // ########## Step 1 - set up common objects to be used in the program

  // Define the width and height of the framebuffer
  // image size
  vec2i imgSize;
  imgSize.x = 1024;  // width
  imgSize.y = 768;   // height

  // camera
  // Placing the camera at the origin <0,0,0>
  float cam_pos[] = {0.f, 0.f, 0.f};

  // Orient the camera noting Y-up
  float cam_up[] = {0.f, 1.f, 0.f};

  // set the camera view direction
  float cam_view[] = {0.1f, 0.f, 1.f};

  // triangle mesh data
  // 4 vertices each with a XYZ position
  float vertex[] = {
    -1.0f, -1.0f, 3.0f, 
    -1.0f, 1.0f, 3.0f,
    1.0f, -1.0f, 3.0f, 
    0.1f,  0.1f, 0.3f};

  // 4 colors denoted by RGBA
  float color[] = {
    0.9f, 0.5f, 0.5f, 1.0f, 
    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f, 
    0.5f, 0.9f, 0.5f, 1.0f};

  // index for the triangles
  int32_t index[] = {0, 1, 2, 1, 2, 3};

  // ########## Step 2 - execute ospInit() to initialize the renderer
    printf("initialize OSPRay...");

  // initialize OSPRay; OSPRay parses (and removes) its commandline parameters,
  OSPError init_error = ospInit(&argc, argv);
  if (init_error != OSP_NO_ERROR)
    return init_error;

  printf("done!\n");
  
  // ########## Step 3 - setup the camera into the scene
  printf("setting up camera...");

  // Feeding the camera data to OSPRay
  // create and setup camera
  OSPCamera camera = ospNewCamera("perspective");
  ospSetFloat(camera, "aspect", imgSize.x / (float)imgSize.y);
  ospSetParam(camera, "position", OSP_VEC3F, cam_pos);
  ospSetParam(camera, "direction", OSP_VEC3F, cam_view);
  ospSetParam(camera, "up", OSP_VEC3F, cam_up);
  ospCommit(camera);  // commit each object to indicate modifications are done

  printf("done!\n");

  // ########## Step 4 - setup the scene
  printf("setting up scene...");

  // #################### Step 4.1 Feeding the model/vertex data to OSPRay
  // create and setup model and mesh
  OSPGeometry mesh = ospNewGeometry("mesh");

  OSPData data = ospNewSharedData1D(vertex, OSP_VEC3F, 4);
  // alternatively with an OSPRay managed OSPData
  // OSPData managed = ospNewData1D(OSP_VEC3F, 4);
  // ospCopyData1D(data, managed, 0);

  ospCommit(data);
  ospSetObject(mesh, "vertex.position", data);
  ospRelease(data);  // we are done using this handle

  data = ospNewSharedData1D(color, OSP_VEC4F, 4);
  ospCommit(data);
  ospSetObject(mesh, "vertex.color", data);
  ospRelease(data);

  data = ospNewSharedData1D(index, OSP_VEC3UI, 2);
  ospCommit(data);
  ospSetObject(mesh, "index", data);
  ospRelease(data);

  ospCommit(mesh);

  // #################### Step 4.2 - create a material 
  OSPMaterial mat = ospNewMaterial("pathtracer", "obj");
  ospCommit(mat);


  // #################### Step 4.3 - put the mesh into a model and apply material
  OSPGeometricModel model = ospNewGeometricModel(mesh);
  ospSetObject(model, "material", mat);
  ospCommit(model);
  ospRelease(mesh);
  ospRelease(mat);

  // #################### Step 4.4 - put the model into a group (collection of models)
  OSPGroup group = ospNewGroup();
  ospSetObjectAsData(group, "geometry", OSP_GEOMETRIC_MODEL, model);
  ospCommit(group);
  ospRelease(model);

  /// #################### Step 4.5 - put the group into an instance (give the group a world transform)
  OSPInstance instance = ospNewInstance(group);
  ospCommit(instance);
  ospRelease(group);

  // #################### Step 4.6 - put the instance in the world
  OSPWorld world = ospNewWorld();
  ospSetObjectAsData(world, "instance", OSP_INSTANCE, instance);
  ospRelease(instance);

  // #################### Step 4.7 - create and setup light for Ambient Occlusion
  OSPLight light = ospNewLight("ambient");
  ospCommit(light);
  ospSetObjectAsData(world, "light", OSP_LIGHT, light);
  ospRelease(light);

  ospCommit(world);

  printf("done!\n");

  // print out world bounds
  OSPBounds worldBounds = ospGetBounds(world);
  printf("\nworld bounds: ({%f, %f, %f}, {%f, %f, %f}\n\n",
         worldBounds.lower[0], worldBounds.lower[1], worldBounds.lower[2],
         worldBounds.upper[0], worldBounds.upper[1], worldBounds.upper[2]);


  // ########## Step 5 - setup of the renderer
  printf("setting up renderer...");

  // #################### Step 5.1 - create renderer
  OSPRenderer renderer =
      ospNewRenderer("pathtracer");  // choose path tracing renderer

  // complete setup of renderer
  ospSetFloat(renderer, "backgroundColor", 1.0f);  // white, transparent
  ospCommit(renderer);

  // #################### Step 5.2 - create and setup framebuffer
  OSPFrameBuffer framebuffer =
      ospNewFrameBuffer(imgSize.x, imgSize.y, OSP_FB_SRGBA,
                        OSP_FB_COLOR | /*OSP_FB_DEPTH |*/ OSP_FB_ACCUM);
  ospResetAccumulation(framebuffer);

  printf("rendering initial frame to firstFrame.png...");

  // #################### Step 5.3 - render one frame
  ospRenderFrameBlocking(framebuffer, renderer, camera, world);

  // #################### Step 5.4 - access framebuffer and write its content as PNG file
  const uint32_t *fb = (uint32_t *)ospMapFrameBuffer(framebuffer, OSP_FB_COLOR);
  writePNG("firstFrame.png", imgSize, fb);
  ospUnmapFrameBuffer(fb, framebuffer);

  printf("done!\n");

  ospUnmapFrameBuffer(fb, framebuffer);

  printf("done!\n");

  // ########## Step 6 - shutdown system and clean up
  printf("\ncleaning up objects...");

  // final cleanups
  ospRelease(renderer);
  ospRelease(camera);
  ospRelease(framebuffer);
  ospRelease(world);

  printf("done!\n");

  ospShutdown();

  return 0;
}
