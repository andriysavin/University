# Course Work: Particle System

This project implements a [particle system](https://en.wikipedia.org/wiki/Particle_system) algorithm, which was my computer graphics course work task. It shows a 3D scene, the key part of which is steam coming out of a teapot. The steam is simulated using the particle system. The user can move the camera, and can change the "temperature" of the teapot (which results in steam intencity change). Rendering is based on DirectX [point sprites](https://learn.microsoft.com/en-us/windows/win32/direct3d9/point-sprites), which basically is DirectX's ability to render textured squares which always face the camera, while calling code provides center coordinates, scale and the texture. 

![ParticleSystem Demo](ParticleSystem%20Demo.gif)

The code (written in 2003) was upgraded to be compatible with VS 2022 and modern C++. DirectX v8 was upgraded to v9. D3DX library went out of support since then, so it was added as a NuGet package. Besides that, it preserves the code style and student-level quality 👨🏻‍🎓 After all, the goal was to demonstrate knowledge of principles of computer graphics along with math and geomety, not perfect C++! Today, I struggle to explain some pieces of my own code here 😅

As a side-note, at around that time I began my first job at a [gaming startup](http://store.steampowered.com/app/289200/), where I worked on a 3D graphics game engine. One of the tasks was implementing various visual effects (e.g. for battling scenes: fireballs, casting magic spells, explosions etc.) via particle systems. So this course work was a rather simplified variant of what actually went to the game.
