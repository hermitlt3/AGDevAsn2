function SaveToLuaFile(outputString, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Image/SavedData.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Image/SavedData.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end

function RewriteToLuaVariable(outputString)
	local f;
	f:write(outputString);
end

-- CalculateDistanceSquare
function CalculateDistanceSquare(x1, y1, z1, x2, y2, z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
	print(distanceSquare)
	return distanceSquare
end

function TestSum(...)
	local total = 0;
	for i,v in ipairs(arg) do
		total = total + v
    end
	print(total)
end

title = "DM2240 - Week 14 LuaScripting"
width = 800
height = 600

-- Keyboard Inputs
moveForward 	= "W"
moveBackward 	= "S"
moveLeft		= "A"
moveRight		= "D"
reload			= "R"
jump			= 32
secondary		= 1
primary			= 0

-- CPlayerInfo start position
CPlayerInfoStartPos = {400, 0, 0}
CPlayerInfoStartTarget = {0, 0, 0}
CPlayerInfoStartUp = {0, 1, 0}

-- Way points
Waypoint_A_1 = {x=10.0, y=0.0, z=50.0}
Waypoint_A_2 = {x=10.0, y=0.0, z=-50.0}
Waypoint_A_3 = {x=-10.0, y=0.0, z=0.0}