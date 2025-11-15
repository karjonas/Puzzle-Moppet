add_library(Litha STATIC
    Animators/BobAnimator.cpp
    Animators/BobAnimator.h
    Animators/RotationAnimator.cpp
    Animators/RotationAnimator.h
    Camera.cpp
    Camera.h
    CGUITTFont.cpp
    CGUITTFont.h
    Character.cpp
    Character.h
    Colors.cpp
    Engine.cpp
    Engine.h
    EventQueue.cpp
    EventQueue.h
    InfiniteRunningAverage.h
    InputProfile.cpp
    InputProfile.h
    Kernel.cpp
    Kernel.h
    LogicTask.cpp
    LogicTask.h
    Mesh.cpp
    Mesh.h
    MotionSensor.cpp
    MotionSensor.h
    NodeHandler.cpp
    NodeHandler.h
    Physics/BoxCollisionGeometry.cpp
    Physics/BoxCollisionGeometry.h
    Physics/CollisionGeometry.cpp
    Physics/CollisionGeometry.h
    Physics/CollisionMaterial.cpp
    Physics/CollisionMaterial.h
    Physics/DynamicBody.cpp
    Physics/DynamicBody.h
    Physics/MeshCollisionGeometry.cpp
    Physics/MeshCollisionGeometry.h
    Physics/ode_utility.cpp
    Physics/ode_utility.h
    Physics/Physics.cpp
    Physics/Physics.h
    Physics/ray_collision.cpp
    Physics/ray_collision.h
    Physics/SphereCollisionGeometry.cpp
    Physics/SphereCollisionGeometry.h
    Physics/StaticBody.cpp
    Physics/StaticBody.h
    PostProcessingChain.cpp
    PostProcessingChain.h
    ProxyTransformable.cpp
    ProxyTransformable.h
    RenderTask.cpp
    RenderTask.h
    Shader.cpp
    Shader.h
    ShaderInstance.cpp
    ShaderInstance.h
    ShaderInstanceDef.h
    ShaderManager.cpp
    ShaderManager.h
    SoundMotionCallback.h
    SoundQueue.cpp
    SoundQueue.h
    SoundSource.cpp
    SoundSource.h
    SoundSystems/OpenALSoundSystem
    SoundSystems/OpenALSoundSystem/openal_stuff.cpp
    SoundSystems/OpenALSoundSystem/openal_stuff.h
    SoundSystems/OpenALSoundSystem/OpenALSound.cpp
    SoundSystems/OpenALSoundSystem/OpenALSound.h
    SoundSystems/OpenALSoundSystem/OpenALSound2D.cpp
    SoundSystems/OpenALSoundSystem/OpenALSound2D.h
    SoundSystems/OpenALSoundSystem/OpenALSound3D.cpp
    SoundSystems/OpenALSoundSystem/OpenALSound3D.h
    SoundSystems/OpenALSoundSystem/OpenALSoundSystem.cpp
    SoundSystems/OpenALSoundSystem/OpenALSoundSystem.h
    SoundSystems/OpenALSoundSystem/sound_reg.cpp
    SoundSystems/OpenALSoundSystem/sound_reg.h
    SoundSystems/OpenALSoundSystem/stb_vorbis.c
    SoundSystems/OpenALSoundSystem/stb_vorbis.h
    SoundSystems/SoundSystem.cpp
    SoundSystems/SoundSystem.h
    Task.h
    ThirdPersonCameraController.cpp
    ThirdPersonCameraController.h
    Updater.h
    UserCharacterController.cpp
    UserCharacterController.h
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
    World.h
)

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

find_package(Freetype REQUIRED)
list(APPEND LINK_LIBRARIES Freetype::Freetype)

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
