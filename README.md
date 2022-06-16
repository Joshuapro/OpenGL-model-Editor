

## Key Summary Description

* Object creation: Keys 1, 2, 3 for creating a "cube", "rabbit", "bumpy object"
* Selection: Mouse click on object for selection
* T for delete
* P, F, N for phong shading, flat shading, wireframe
* E, R, I, O for rotating the object in 4 directions
* W, S, A, D for object movement
* G, H for scale up and down of object
* Key Up for switching between orth cam and perspective
* Z, X, C, V for camera movement (ortho, persp)



## Object Creation.



![Screen Shot 2021-11-09 at 6 48 30 PM](https://user-images.githubusercontent.com/35964921/141023816-a1119f8b-4441-495b-8836-b55dd4effa7c.png)

I have 3 vectors of std::vector<Cube> cubes; std::vector<Bunny> bunnies; std::vector<Bumpy> bumpies, that stored each type of object. For the bunny and the bumpy cube object, I had to read a .off file when the obj creation is called.
  
  
 ## Selection.



 
![Screen Shot 2021-11-09 at 6 59 32 PM](https://user-images.githubusercontent.com/35964921/141024841-0bf48b39-3c94-4f0e-bd84-050b7fee0b39.png)
![Screen Shot 2021-11-09 at 6 59 26 PM](https://user-images.githubusercontent.com/35964921/141024856-bb0dac44-491a-47c9-b084-1cfe936628b9.png)
  
When clicking on the object, the object is selected. Once it is selected, the color changes to green.
  
I utilized the stencil buffer for object selection. The stencil buffer draws each object as id's onto the screen. Each object has its own ID, and when I click on the screen, I check if what I clicked is a match to any id in all my objects.
  
<img width="321" alt="Screen Shot 2021-11-09 at 7 04 47 PM" src="https://user-images.githubusercontent.com/35964921/141025520-bcebd594-1c26-4af3-8179-5a9970cedb69.png">
  
<img width="582" alt="Screen Shot 2021-11-09 at 7 09 07 PM" src="https://user-images.githubusercontent.com/35964921/141025723-f798da02-69b7-428e-943a-8a3e64de4d1b.png">

 ^ What clicked is stored into globalpickedid

  
  
 ## Delete. 


https://user-images.githubusercontent.com/35964921/141025088-1b910351-91f4-4abb-a3fb-440c2cef5ec4.MOV
  
  
 ## Rendering Setting.
  
 
https://user-images.githubusercontent.com/35964921/141026815-4e7e94fd-b258-41ae-8e4a-b8a4701a722f.MOV


  After Selecting an object, I pressed between P, F, N for changing the object render.


## Rotation.

https://user-images.githubusercontent.com/35964921/141027180-3ed25ca2-0c87-482a-b4e0-e1c85bca8e80.MOV

For each object, I had a angle x and angle y associated with it. So every time I render it, it would use the x and y adjustment.
  
## Object Movement.

https://user-images.githubusercontent.com/35964921/141030997-b1ea2e93-4cbf-4f04-9868-d8d89b54fdff.MOV
  
 Each Object has a `glm::vec3 modelpos;` assocaited
 
 This is uploaded to the gpu later `auto model = glm::translate(glm::mat4(1.0f), objects[i].modelpos);`


## Scale.

https://user-images.githubusercontent.com/35964921/141031002-1278d23a-bd30-4bdd-86c7-729a3a61d321.MOV
  
 Each Object has a `objsize` assocaited
  
later `model = glm::scale(model, glm::vec3(objsize, objsize, objsize));`
 
  

 
  
## Orthographic Camera.

https://user-images.githubusercontent.com/35964921/141032245-cc1a0445-6047-450d-9e2a-07932805765e.MOV




 ## Perspective Camera.

 
https://user-images.githubusercontent.com/35964921/141032258-4b893c71-242d-4e39-89fb-ee3f56c35a16.MOV


For Both Orthographic Camera and Perspective Camera, I made another matrix called projection and uploaded it to the gpu.


