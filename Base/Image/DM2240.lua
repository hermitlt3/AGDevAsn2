function SaveToLuaFile(outputString, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Image/DM2240_HighScore.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
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
inventory		= "I"


-- CPlayerInfo start position
CPlayerInfoStartPos = {0, 0, 10}

-- CalculateDistanceSquare
function CalculateDistanceSquare(x1, y1, z1, x2, y2, z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
	print(distanceSquare)
	return distanceSquare
end

function TestSum(a, b, c, d)
	local la = a + 2
	local lb = b + 2
	local lc = c + 2
	local ld = d + 2
	print(la + lb + lc + ld)
end