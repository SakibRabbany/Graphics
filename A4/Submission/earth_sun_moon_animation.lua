-- A simple scene with some miscellaneous geometry.


mat1 = gr.material({0.7, 1.0, 0.7}, {0, 0, 0}, 25, {0,0,0,0}, 0)
mat2 = gr.material({0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, 25, {0,0,0,0}, 0)
mat3 = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, {0,0,0,0}, 0)
mat4 = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, {0,0,0,0}, 0)

scene_root = gr.node('root')
scene_root:translate(170,-100,0)

sun = gr.mesh( 'sun', 'smstdodeca.obj' )
sun:set_material(mat3)
scene_root:add_child(sun)
--sun:scale(60,60,60)
--sun:translate(-2000, 10000, -50000)

earth = gr.sphere('earth')
sun:add_child(earth)
earth:set_material(mat1)
earth:set_texture("Submission/earth.png")
earth:scale(100,100,100)
earth:translate(100,0,0)

moon = gr.sphere('moon')
earth:add_child(moon)
moon:set_material(mat1)
moon:set_texture("Submission/moon.png")
--moon:scale(1/100,1/100,1/100)
--moon:scale(30,30,30)
moon:translate(-400,0,100)


-- A small stellated dodecahedron.



white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
orange_light = gr.light({400.0, 100.0, 150.0}, {0.7, 0.0, 0.7}, {1, 0, 0})

-- gr.render(scene_root, 'earth_sun_moon_animation.png', 500, 500,
-- 	  {0, 0, 1000}, {0, 0, -1}, {0, 1, 0}, 50,
-- 	  {0.3, 0.3, 0.3}, {white_light})
