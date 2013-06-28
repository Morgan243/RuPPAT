print('-------RuPPAT Lua Init Script Loaded---------')

--set main config information (resolution and background)
main = {}
main["width"] = 980
main["height"] = 900

--load the background sprites
main["backgrounds"] = {}
main["backgrounds"][0] = "bkg_one_1080.png"
main["backgrounds"][1] = "bkg_two_1080.png"
main["backgrounds"][2] = "bkg_three_1080.png"


--main user's player information
players = {}
players["amount"] = 1

player0 = {}
player0["sprite"] = "red_ship_wraith.png"
player0["x"] = 100
player0["y"] = 100
player0["HC"] = "red_ship_HC.png"

--give player zero a missile
player0["missile"] = {}
player0["missile"]["sprite"] = "white_rocket.png"
player0["missile"]["name"] = "White Rocket"
player0["missile"]["amount"] = 100
player0["missile"]["damage"] = 10
player0["missile"]["velocity"] = 40
player0["missile"]["lifespan"] = 4


players[0] = player0


--define some asteroids to put into objects
asteroid_medium = {}
asteroid_medium["x"] = 300
asteroid_medium["y"] = 500
asteroid_medium["mass"] = 150
asteroid_medium["rotation"] = -1.3
asteroid_medium["x velocity"] = 10
asteroid_medium["y velocity"] = 20
asteroid_medium["sprite"] = "asteroid_medium_1.png"

objects = {}
objects["amount"] = 3
--set first asteroid
objects[0] = asteroid_medium


asteroid_medium["x"] = 1400
asteroid_medium["y"] = 300
asteroid_medium["mass"] = 100
asteroid_medium["rotation"] = -0.3
asteroid_medium["x velocity"] = -30
asteroid_medium["y velocity"] = 2

--set second asteroid
objects[1] = asteroid_medium


asteroid_medium["x"] = 400
asteroid_medium["y"] = 200
asteroid_medium["mass"] = 600
asteroid_medium["rotation"] = 0.5
asteroid_medium["x velocity"] = -5
asteroid_medium["y velocity"] = 10
asteroid_medium["sprite"] = "asteroid_large_1.png"

--set third and final asteroid
objects[2] = asteroid_medium
