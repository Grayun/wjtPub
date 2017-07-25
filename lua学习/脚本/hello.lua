
-- WjtLuaArxPubRunLua
PrintArx("\nhello arxLua")
--tbale 测试 .. 为字符串连接 
tab1 = { key1 = "vall", key2 = "val2", "val3"}
for k, v in pairs(tab1) do
	PrintArx("\n"..k .. " - " .. v)
end

pt1 = AcGePoint3d(100, 0, 0)
pt2 = AcGePoint3d(10, 0, 0)
--PrintArxLn(TCHARTochar(string.format("是否相等"))
PrintArxLn(pt1:distanceTo(pt2))
bL = false
PrintArxLn(string.format("b1:%d", b1))
bl = pt1:isEqualTo(pt2, CgeTol)
PrintArxLn(string.format("b1111:%d", b1))
PrintArxLn("sefef")
string.format("b1:%d", b1)
PrintArxLn(bl)
PrintArxLn("111")
--strPrompt =   
