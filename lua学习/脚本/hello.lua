
-- WjtLuaArxPubRunLua
PrintArx("\nhello arxLua")
--tbale 测试 .. 为字符串连接 
tab1 = { key1 = "vall", key2 = "val2", "val3"}
for k, v in pairs(tab1) do
	PrintArx("\n"..k .. " - " .. v)
end

pt1 = AcGePoint3d(100, 0, 0)
pt2 = AcGePoint3d(10, 0, 0)
--PrintArxLn(string.format("是否相等"))
PrintArxLn(pt1:distanceTo(pt2))
PrintArxLn("相等")
--strPrompt =   pt1:isEqualTo(pt2)
