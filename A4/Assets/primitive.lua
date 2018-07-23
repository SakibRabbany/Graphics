-- test for hierarchical ray-tracers.
-- Thomas Pflaum 1996

gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 25, {0,0,0,0}, 0.27)
grass = gr.material({0.1, 0.7, 0.1}, {0.0, 0.0, 0.0}, 0, {0,0,0,0}, 0.27)
blue = gr.material({0.7, 0.6, 1}, {0.5, 0.4, 0.8}, 25, {0,0,0,0}, 0.05)


red_wall = gr.material( {0.44, 0.137, 0.1886}, {0, 0, 0}, 25, {0,0,0,0}, 0.0)
green_wall = gr.material( {0.1386, 0.1327, 0.45}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
grey_wall = gr.material( {0.4649, 0.4823, 0.5002}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)
light_wall = gr.material( {0.223, 0.498, 0.858}, {0, 0, 0}, 25,{0,0,0,0}, 0.0)

mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25,{0,0,0,0}, 0.0)	
mat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25,{0,0,0,0}, 0.0)		

scene = gr.node('scene')
--scene:rotate('X', 23)
scene:translate(0, 0, 0)

-- the arc
--arc = gr.node('arc')
--scene:add_child(arc)
--arc:translate(0,0,-10)
--arc:rotate('Y', 60)


--p1 = gr.cube('p1')
--arc:add_child(p1)
--p1:set_material(gold)
--p1:scale(0.8, 4, 0.8)
--p1:translate(-2.4, 0, -0.4)

--p2 = gr.cube('p2')
--arc:add_child(p2)
--p2:set_material(gold)
--p2:scale(0.8, 4, 0.8)
--p2:translate(1.6, 0, -0.4)

b1 = gr.cube('b1')
scene:add_child(b1)
b1:scale(2, 2, 2)
b1:set_material(green_wall)
--b1:set_texture("Assets/sample3.png")
b1:translate(-2, 0, -6)

s2 = gr.sphere('s2')
scene:add_child(s2)
s2:set_material(grey_wall)
-- s2:set_texture("Assets/sample4.png")
s2:scale(1, 1, 1)
s2:translate(-5, 1, -5)

cone = gr.cone('cone')
scene:add_child(cone)
cone:set_material(red_wall)
cone:scale(1, 2, 1)
cone:translate(-8, 0, -5)

cylinder = gr.cylinder('cylinder')
scene:add_child(cylinder)
cylinder:set_material(light_wall)
cylinder:scale(1, 1, 1)
cylinder:translate(-11, 0, -5)




--s = gr.sphere('s')
--arc:add_child(s)
--s:set_material(gold)
--s:scale(4, 0.6, 0.6)
--s:translate(0, 4, 0)

-- the floor
plane = gr.cube( 'plane' )
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)

-- sphere
--poly = gr.mesh( 'poly', 'dodeca.obj' )
--scene:add_child(poly)
--poly:translate(-2, 1.618034, 0)
--poly:set_material(blue)

--cylinder = gr.cylinder('cylinder')
--scene:add_child(cylinder)
--cylinder:set_material(blue)
--cylinder:scale(5, 2, 5)
--cylinder:translate(-5, 4, -10)



-- The lights
l1 = gr.light({200,200,400}, {0.8, 0.8, 0.8}, {1, 0, 0})
l2 = gr.light({-200,200,-400}, {0.4, 0.4, 0.8}, {1, 0, 0})

gr.render(scene, 'primitive.png', 512, 512, 
	  {0, 5, 0,}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {l1, l2})
