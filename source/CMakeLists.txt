add_library(Litha STATIC
    # C++ files
    Animators/BobAnimator.cpp
    Animators/RotationAnimator.cpp
    Camera.cpp
    Character.cpp
    Colors.cpp
    Engine.cpp
    EventQueue.cpp
    InputProfile.cpp
    Kernel.cpp
    LogicTask.cpp
    Mesh.cpp
    MotionSensor.cpp
    NodeHandler.cpp
    Physics/BoxCollisionGeometry.cpp
    Physics/CollisionGeometry.cpp
    Physics/CollisionMaterial.cpp
    Physics/DynamicBody.cpp
    Physics/MeshCollisionGeometry.cpp
    Physics/ode_utility.cpp
    Physics/Physics.cpp
    Physics/ray_collision.cpp
    Physics/SphereCollisionGeometry.cpp
    Physics/StaticBody.cpp
    PostProcessingChain.cpp
    ProxyTransformable.cpp
    RenderTask.cpp
    Shader.cpp
    ShaderInstance.cpp
    ShaderManager.cpp
    SoundQueue.cpp
    SoundSource.cpp
    SoundSystems/OpenALSoundSystem/OpenALSound2D.cpp
    SoundSystems/OpenALSoundSystem/OpenALSound3D.cpp
    SoundSystems/OpenALSoundSystem/OpenALSound.cpp
    SoundSystems/OpenALSoundSystem/OpenALSoundSystem.cpp
    SoundSystems/OpenALSoundSystem/openal_stuff.cpp
    SoundSystems/OpenALSoundSystem/sound_reg.cpp
    SoundSystems/SoundSystem.cpp
    ThirdPersonCameraController.cpp
    UserCharacterController.cpp
    utils/file.cpp
    utils/log.cpp
    utils/maths.cpp
    utils/os.cpp
    utils/os/path.cpp
    utils/paths.cpp
    utils/str.cpp
    utils/Variant.cpp
    utils/VariantMap.cpp
    World.cpp
    # C files
    SoundSystems/OpenALSoundSystem/stb_vorbis.c)

if(NOT BUILD_FOR_APPIMAGE AND NOT BUILD_FOR_PKGBUILD)
    # If building for appimage we use system openal
    find_package(OpenAL CONFIG REQUIRED)
endif()

if(NOT BUILD_FOR_PKGBUILD)
    find_package(irrlicht CONFIG REQUIRED 1.7)
    find_package(ode CONFIG REQUIRED)
endif()

set(LINK_LIBRARIES Irrlicht)
# If building for appimage we use system openal
if(BUILD_FOR_APPIMAGE OR BUILD_FOR_PKGBUILD)
    list(APPEND LINK_LIBRARIES openal)
else()
    list(APPEND LINK_LIBRARIES OpenAL::OpenAL)
endif()

set(IRRLICHT_INCLUDE_DIR "")
if(BUILD_FOR_PKGBUILD)
    list(APPEND LINK_LIBRARIES ode)
    set(IRRLICHT_INCLUDE_DIR /usr/include/irrlicht)
else()
    list(APPEND LINK_LIBRARIES ODE::ODE)
endif()

target_link_libraries(Litha ${LINK_LIBRARIES})

target_include_directories(Litha PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    .
    Animators
    Physics
    SoundSystems
    SoundSystems/OpenALSoundSystem
    utils
    utils/os
    ${IRRLICHT_INCLUDE_DIR}
    )
