print('-------RuPPAT Lua Init Script Loaded---------')

--set main config information (resolution and background)
main = {}
main["width"] = 980
main["height"] = 900
main["font"] = "FontSheet.png"

--load the background sprites
main["backgrounds"] = {}
main["backgrounds"]["amount"] = 3
main["backgrounds"][0] = "bkg_one_1080.png"
main["backgrounds"][1] = "bkg_two_1080.png"
main["backgrounds"][2] = "bkg_three_1080.png"


--main user's player information
players = {}
players["amount"] = 1

player0 = {}
player0["sprite"] = "red_ship_wraith.png"
player0["x"] = 100
player0["y"] = 110
player0["HC"] = "red_ship_HC.png"
player0["max accel"] = 0.5

--give player zero some missiles
player0["missiles"] = {}
player0["missiles"]["amount"] = 1

player0["missiles"][0] = {}
player0["missiles"][0]["sprite"] = "white_rocket.png"
player0["missiles"][0]["name"] = "White Rocket"
player0["missiles"][0]["amount"] = 100
player0["missiles"][0]["damage"] = 10
player0["missiles"][0]["velocity"] = 40
player0["missiles"][0]["lifespan"] = 4


players[0] = player0

objects = {}
objects["amount"] = 4

--define some asteroids to put into objects
asteroid_medium = {}
asteroid_medium["x"] = 300
asteroid_medium["y"] = 500
asteroid_medium["mass"] = 150
asteroid_medium["rotation"] = -1.3
asteroid_medium["x velocity"] = 10
asteroid_medium["y velocity"] = 0
asteroid_medium["sprite"] = "asteroid_medium_1.png"
asteroid_medium["HC"] = "asteroid_medium_1.png"


--define some asteroids to put into objects
asteroid_medium_1 = {}
asteroid_medium_1["x"] = 1000
asteroid_medium_1["y"] = 200
asteroid_medium_1["mass"] = 150
asteroid_medium_1["rotation"] = -0.3
asteroid_medium_1["x velocity"] = 10
asteroid_medium_1["y velocity"] = -10
asteroid_medium_1["sprite"] = "asteroid_medium_1.png"
asteroid_medium_1["HC"] = "asteroid_medium_1.png"



--define some asteroids to put into objects
asteroid_large = {}
asteroid_large["x"] = 300
asteroid_large["y"] = 100
asteroid_large["mass"] = 150
asteroid_large["rotation"] = -1.1
asteroid_large["x velocity"] = 20
asteroid_large["y velocity"] = 5
asteroid_large["sprite"] = "asteroid_large_1.png"
asteroid_large["HC"] = "asteroid_large_1_HC.png"




--set first asteroid
objects[0] = asteroid_medium

--set second asteroid
objects[1] = asteroid_medium_1

--set third and final asteroid
objects[2] = asteroid_large

red_planet = {}
red_planet["x"] = 850
red_planet["y"] = 550
red_planet["mass"] = 1500
red_planet["rotation"] = -2.0
red_planet["x velocity"] = 0
red_planet["y velocity"] = 0
red_planet["sprite"] = "red_planet.png"
red_planet["HC"] = "red_planet_HC.png"

objects[3] = red_planet


