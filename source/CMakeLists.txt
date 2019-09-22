add_library(Litha STATIC
    # C++ files
    Animators/BobAnimator.cpp
    Animators/RotationAnimator.cpp
    Camera.cpp
    Character.cpp
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

target_include_directories(Litha PUBLIC
    ./
    Animators
    Physics
    SoundSystems
    SoundSystems/OpenALSoundSystem
    utils
    utils/os)

target_link_libraries(Litha ${IRRLICHT_LIBRARIES})

if (WIN32)
	target_link_libraries(Litha ode OpenAL32)
endif (WIN32)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(Litha ode openal)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_link_libraries(Litha ode /System/Library/Frameworks/OpenAL.framework/OpenAL /System/Library/Frameworks/OpenGL.framework/OpenGL /System/Library/Frameworks/Cocoa.framework/Cocoa /System/Library/Frameworks/Carbon.framework/Carbon /System/Library/Frameworks/IOKit.framework/IOKit)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

