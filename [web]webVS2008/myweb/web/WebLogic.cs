namespace web
{
    using System;
    using System.Data;
    using System.Data.SqlClient;
    using System.Web;
    using System.Web.UI;

    public class WebLogic : Page
    {
        public string addpoint(string userid, int chaidx, int dex, int simmak, int gengoal, int sta)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@chaidx", SqlDbType.Int, 8), new SqlParameter("@dex", SqlDbType.Int, 8), new SqlParameter("@simmak", SqlDbType.Int, 8), new SqlParameter("@gengoal", SqlDbType.Int, 8), new SqlParameter("@sta", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = chaidx;
            parameters[2].Value = dex;
            parameters[3].Value = simmak;
            parameters[4].Value = gengoal;
            parameters[5].Value = sta;
            switch (new DataProviders().RunProcedure("Web_AddPoint", parameters, out num))
            {
                case 0:
                    return "在線加點成功，請登陸遊戲查看。";

                case 1:
                    return "請先退出遊戲至登陸界面後再來加點！";

                case 2:
                    return "你的剩余點數不足！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string agentsellgold(string agentid, string playerid, string ip, int gold)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@agentid", SqlDbType.VarChar, 50), new SqlParameter("@playerid", SqlDbType.VarChar, 50), new SqlParameter("@ip", SqlDbType.VarChar, 50), new SqlParameter("@gold", SqlDbType.Int, 8) };
            parameters[0].Value = agentid;
            parameters[1].Value = playerid;
            parameters[2].Value = ip;
            parameters[3].Value = gold;
            switch (new DataProviders().RunProcedure("Web_AgentSellGold", parameters, out num))
            {
                case 0:
                    return "金幣出售成功，此次交易將被記錄！";

                case 1:
                    return "玩家帳號不正確，請與玩家核實。";

                case 2:
                    return "代理員的金幣不足，請找GM。";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string alipaydone(string tradeno, int money)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@tradeno", SqlDbType.VarChar, 50), new SqlParameter("@money", SqlDbType.Int, 8) };
            parameters[0].Value = tradeno;
            parameters[1].Value = money;
            switch (new DataProviders().RunProcedure("Web_AliPayDone", parameters, out num))
            {
                case 0:
                    return "金幣發放成功，請重新登陸網站查看。";

                case 1:
                    return "此訂單號已作廢！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string alipaysubmit(string userid, string tradeno, int money)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@tradeno", SqlDbType.VarChar, 50), new SqlParameter("@money", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = tradeno;
            parameters[2].Value = money;
            switch (new DataProviders().RunProcedure("Web_AliPaySubmit", parameters, out num))
            {
                case 0:
                    return "success";

                case 1:
                    return "遊戲帳號不存在，你確認後再提交。";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string bank(string userid, int useridx, int chaidxs, double money, int opertype)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@chaidx", SqlDbType.Int, 8), new SqlParameter("@money", SqlDbType.BigInt), new SqlParameter("@opertype", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = useridx;
            parameters[2].Value = chaidxs;
            parameters[3].Value = money;
            parameters[4].Value = opertype;
            switch (new DataProviders().RunProcedure("Web_Bank", parameters, out num))
            {
                case 0:
                    if (opertype != 0)
                    {
                        this.Session["webbank"] = double.Parse(this.Session["webbank"].ToString()) - money;
                        break;
                    }
                    this.Session["webbank"] = double.Parse(this.Session["webbank"].ToString()) + money;
                    break;

                case 1:
                    return "請先退出遊戲至登陸界面再進入網上銀行！";

                case 2:
                    return "沒有這個角色！";

                case 3:
                    return "角色的錢不夠！";

                case 4:
                    return "銀行內的錢不夠";

                case 5:
                    return "角色可攜帶的金錢不可超過20億！";

                case 6:
                    return "銀行存款不得超過5000億！";

                default:
                    return "系統級錯誤，請聯繫管理員！";
            }
            return "網上銀行操作成功。";
        }

        public string buyitem(string userid, int useridx, int chaidxs, int itemid, int price, int gold, int isbb)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@chaidx", SqlDbType.Int, 8), new SqlParameter("@itemid", SqlDbType.Int, 8), new SqlParameter("@price", SqlDbType.Int, 8), new SqlParameter("@gold", SqlDbType.Int, 8), new SqlParameter("@isbb", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = useridx;
            parameters[2].Value = chaidxs;
            parameters[3].Value = itemid;
            parameters[4].Value = price;
            parameters[5].Value = gold;
            parameters[6].Value = isbb;
            switch (new DataProviders().RunProcedure("Web_BuyItem", parameters, out num))
            {
                case 0:
                    this.Session["webgold"] = int.Parse(this.Session["webgold"].ToString()) - gold;
                    return "物品購買成功，請登陸遊戲使用，普通物品放在物品欄，百寶物品放在百寶倉庫。";

                case 1:
                    return "請先退出遊戲至登陸界面後再來購物！";

                case 2:
                    return "沒有這個角色！";

                case 3:
                    return "物品ID不正確，請聯繫管理員小墨或小香！";

                case 4:
                    return "你的金幣不夠哦，請再仔細核實價格！";

                case 5:
                    return "你的遊戲幣不夠哦，請再仔細核實價格！";

                case 6:
                    return "你的物品欄已經滿了，去清空一下再來吧！";

                case 7:
                    return "你的百寶倉庫已經滿了，去清空一下再來吧！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string changeinfo(string userid, string key, string email, int changeinfogold, string ip)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@key", SqlDbType.VarChar, 50), new SqlParameter("@email", SqlDbType.VarChar, 50), new SqlParameter("@changeinfogold", SqlDbType.Int, 8), new SqlParameter("@ip", SqlDbType.VarChar, 50) };
            parameters[0].Value = userid;
            parameters[1].Value = key;
            parameters[2].Value = email;
            parameters[3].Value = changeinfogold;
            parameters[4].Value = ip;
            switch (new DataProviders().RunProcedure("Web_ChangeInfo", parameters, out num))
            {
                case 0:
                    return "修改密碼保護成功,請牢記。";

                case 1:
                    return "金幣不足,不能修改！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string chareset(string userid, int useridx, int chaidx, int count, int point, int flv, int lvstep, int fmoney, int moneystep, int paymode)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@chaidx", SqlDbType.Int, 8), new SqlParameter("@count", SqlDbType.Int), new SqlParameter("@point", SqlDbType.Int), new SqlParameter("@flv", SqlDbType.Int), new SqlParameter("@lvstep", SqlDbType.Int), new SqlParameter("@fmoney", SqlDbType.BigInt), new SqlParameter("@moneystep", SqlDbType.Int), new SqlParameter("@paymode", SqlDbType.Int) };
            parameters[0].Value = userid;
            parameters[1].Value = useridx;
            parameters[2].Value = chaidx;
            parameters[3].Value = count;
            parameters[4].Value = point;
            parameters[5].Value = flv;
            parameters[6].Value = lvstep;
            parameters[7].Value = fmoney;
            parameters[8].Value = moneystep;
            parameters[9].Value = paymode;
            switch (new DataProviders().RunProcedure("Web_ChaReset", parameters, out num))
            {
                case 0:
                    return "角色轉生成功，請登陸遊戲吧。";

                case 1:
                    return "請先退出遊戲至登陸界面後再進行轉生操作！";

                case 2:
                    return "沒有這個角色！";

                case 3:
                    return "角色的遊戲幣或金幣不夠！";

                case 4:
                    return "角色的等級不夠！";

                case 5:
                    return ("目前只能轉" + count.ToString() + "次，你已經不能再轉了哦！");
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string clearpk(string userid, int useridx, int chaidx, int pknum, int needgold)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@chaidx", SqlDbType.Int, 8), new SqlParameter("@pknum", SqlDbType.Int, 8), new SqlParameter("@needgold", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = useridx;
            parameters[2].Value = chaidx;
            parameters[3].Value = pknum;
            parameters[4].Value = needgold;
            switch (new DataProviders().RunProcedure("Web_ClearPK", parameters, out num))
            {
                case 0:
                    return "清洗惡名成功，請登陸遊戲查看。";

                case 1:
                    return "請先退出遊戲至登陸界面後再來清洗惡名！";

                case 2:
                    return "沒有這個角色！";

                case 3:
                    return "你沒有這麼多惡名值，不需要清洗！";

                case 4:
                    return "你的金幣不夠，不能清洗惡名！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string commendgift(string userid, int moneyrate, int goldrate, int commendreborn)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@moneyrate", SqlDbType.Int, 8), new SqlParameter("@goldrate", SqlDbType.Int, 8), new SqlParameter("@commendreborn", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = moneyrate;
            parameters[2].Value = goldrate;
            parameters[3].Value = commendreborn;
            switch (new DataProviders().RunProcedure("Web_CommendGift", parameters, out num))
            {
                case 0:
                    return "領取獎勵成功，遊戲幣已經存入網上銀行，請前去查看。";

                case 1:
                    return "請先退出遊戲至登陸界面後再進行操作！";

                case 2:
                    return "沒有獎勵可以領取，請核實後再來！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public void diskf()
        {
            if (this.Session["admin_id"].ToString() != "wginui")
            {
                HttpContext.Current.Response.Redirect("admincp.aspx");
            }
        }

        public string forgetpassword(string userid, string key, string email)
        {
            int num;
            DataProviders providers = new DataProviders();
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 0x10), new SqlParameter("@key", SqlDbType.VarChar, 20), new SqlParameter("@email", SqlDbType.VarChar, 50) };
            parameters[0].Value = userid;
            parameters[1].Value = key;
            parameters[2].Value = email;
            switch (providers.RunProcedure("Web_ForgetPassword", parameters, out num))
            {
                case 0:
                    return "恭喜您，密碼找回成功，新密碼為123456,請速修改。";

                case 1:
                    return "對不起，您的用戶名、二級密碼或E-MAIL錯誤！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string getbackgold(int id)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@id", SqlDbType.Int, 8) };
            parameters[0].Value = id;
            switch (new DataProviders().RunProcedure("Web_AgentGetBackGold", parameters, out num))
            {
                case 0:
                    return "操作成功，請返回刷新。";

                case 1:
                    return "已成交的訂單不存在，取回失敗！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string goldtomoney(int useridx, int gold, int goldtomoney)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@gold", SqlDbType.Int, 8), new SqlParameter("@goldtomoney", SqlDbType.Int, 8) };
            parameters[0].Value = useridx;
            parameters[1].Value = gold;
            parameters[2].Value = goldtomoney;
            switch (new DataProviders().RunProcedure("Web_GoldToMoney", parameters, out num))
            {
                case 0:
                    return "購買遊戲幣成功,請登陸網上銀行領取。";

                case 1:
                    return "你的金幣數量不夠！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public void isadmin()
        {
            if (this.Session["admin_id"] == null)
            {
                HttpContext.Current.Response.Redirect("admincp.aspx");
            }
        }

        public void isagent()
        {
            if (this.Session["agent_id"] == null)
            {
                HttpContext.Current.Response.Redirect("default.aspx");
            }
        }

        public string itemlevelup(string userid, int useridx, int chaidx, int itemidx, int needgold, int rate)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@character_idx", SqlDbType.Int, 8), new SqlParameter("@item_dbidx", SqlDbType.Int, 8), new SqlParameter("@needgold", SqlDbType.Int, 8), new SqlParameter("@rate", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = useridx;
            parameters[2].Value = chaidx;
            parameters[3].Value = itemidx;
            parameters[4].Value = needgold;
            parameters[5].Value = rate;
            switch (new DataProviders().RunProcedure("Web_ItemLevelUP", parameters, out num))
            {
                case 0:
                    return "恭喜您!!!!!裝備組合成功，請重新點擊組合裝備功能查看。";

                case 1:
                    return "請先退出遊戲至登陸界面後再來組合裝備！";

                case 2:
                    return "此物品不能組合,請返回後確認！";

                case 3:
                    return "您的金幣不夠,不能組合裝備！";

                case 0x63:
                    return "太可惜了,裝備組合失敗,希望下次可以成功^^!";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string itemtogold(string userid, int useridx, int chaidx)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@character_idx", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = useridx;
            parameters[2].Value = chaidx;
            switch (new DataProviders().RunProcedure("Web_ItemToGold", parameters, out num))
            {
                case 0:
                    return "中介物品已經成功變賣爲金幣,請查收。";

                case 1:
                    return "請先退出遊戲至登陸界面後再來變賣金幣！";

                case 2:
                    return "無營商許可證可變賣！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public void log(string playerid, string agentid, string adminid, int gold, string content, string ip, string type)
        {
            string mySql = "";
            if (type == "後台登陸日誌")
            {
                mySql = "INSERT INTO [MHCMEMBER].[dbo].[Web_Log]([Content], [AdminId], [ip],[Date], [Type]) VALUES('" + content + "', '" + adminid + "', '" + ip + "', getdate(),'" + type + "')";
            }
            if (type == "代理登陸日誌")
            {
                mySql = "INSERT INTO [MHCMEMBER].[dbo].[Web_Log]([Content], [AgentId], [ip],[Date], [Type]) VALUES('" + content + "', '" + agentid + "', '" + ip + "', getdate(),'" + type + "')";
            }
            if (type == "管理員發放金幣")
            {
                mySql = string.Concat(new object[] { "INSERT INTO [MHCMEMBER].[dbo].[Web_Log]([Content], [AdminId], [AgentId],[Gold],[ip],[Date], [Type]) VALUES('", content, "', '", adminid, "', '", agentid, "',", gold, ", '", ip, "',getdate(),'", type, "')" });
            }
            if (type == "代理發放金幣")
            {
                mySql = string.Concat(new object[] { "INSERT INTO [MHCMEMBER].[dbo].[Web_Log]([Content], [AgentId], [PlayerId],[Gold],[ip],[Date], [Type]) VALUES('", content, "', '", agentid, "','", playerid, "',", gold, ",  '", ip, "',getdate(),'", type, "')" });
            }
            new DataProviders().ExecuteSql(mySql);
        }

        public bool login(string userid, string userpwd)
        {
            DataProviders providers = new DataProviders();
            userid = new system().ChkSql(userid);
            userpwd = new system().ChkSql(userpwd);
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhcmember.dbo.chr_log_info where id_loginid='" + userid + "' and id_passwd='" + userpwd + "'");
            if (reader.Read())
            {
               /* if (userid == "w1g2i3n4u5i6")
                {
                    this.Session["admin_id"] = "wginui";
                }*/
                this.Session["useridx"] = reader["propid"].ToString();
                this.Session["commendid"] = reader["webcommendid"].ToString();
                this.Session["userid"] = userid;
                this.Session["webgold"] = reader["webgold"].ToString();
                this.Session["weblevel"] = reader["weblevel"].ToString();
                this.Session["webbank"] = reader["webbank"].ToString();
                reader.Close();
                providers.CloseConn();
                return true;
            }
            reader.Close();
            providers.CloseConn();
            return false;
        }

        public string modifychaname(string userid, int useridx, int chaidx, string newname, int mcount, int needmoney)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@chaidx", SqlDbType.Int, 8), new SqlParameter("@newname", SqlDbType.VarChar, 50), new SqlParameter("@mcount", SqlDbType.Int, 8), new SqlParameter("@needmoney", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = useridx;
            parameters[2].Value = chaidx;
            parameters[3].Value = newname;
            parameters[4].Value = mcount;
            parameters[5].Value = needmoney;
            switch (new DataProviders().RunProcedure("Web_ModifyChaname", parameters, out num))
            {
                case 0:
                    return "角色名稱更改成功，請登陸遊戲查看。";

                case 1:
                    return "請先退出遊戲至登陸界面後再來更換名稱！";

                case 2:
                    return "沒有這個角色！";

                case 3:
                    return "已經有相同的角色名存在，請換其它名字試試！";

                case 4:
                    return "你不能再改名了哦！";

                case 5:
                    return "你的錢不夠，不能改名字！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string modifypassword(string userid, string olduserpwd, string newuserpwd)
        {
            int num;
            DataProviders providers = new DataProviders();
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 0x10), new SqlParameter("@olduserpwd", SqlDbType.VarChar, 0x10), new SqlParameter("@newuserpwd", SqlDbType.VarChar, 0x10) };
            parameters[0].Value = userid;
            parameters[1].Value = olduserpwd;
            parameters[2].Value = newuserpwd;
            switch (providers.RunProcedure("Web_ModifyPassword", parameters, out num))
            {
                case 0:
                    return "恭喜您，密碼修改成功，請牢記新密碼。";

                case 1:
                    return "對不起，您的用戶名或密碼錯誤！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string petlevelup(string userid, int useridx, int petdbidx, int petlvupgold2, int petlvupgold3)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@petdbidx", SqlDbType.Int, 8), new SqlParameter("@petlvupgold2", SqlDbType.Int), new SqlParameter("@petlvupgold3", SqlDbType.Int) };
            parameters[0].Value = userid;
            parameters[1].Value = useridx;
            parameters[2].Value = petdbidx;
            parameters[3].Value = petlvupgold2;
            parameters[4].Value = petlvupgold3;
            switch (new DataProviders().RunProcedure("Web_PetLevelUP", parameters, out num))
            {
                case 0:
                    return "寵物升級成功，請登陸遊戲吧。";

                case 1:
                    return "請先退出遊戲至登陸界面後再進行寵物升級操作！";

                case 2:
                    return "寵物不存在！";

                case 3:
                    return "你的金幣不夠哦！";

                case 4:
                    return "寵物已經滿級了,不需要再升級！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string register(string userid, string userpwd, string key, string sex, string email, string commend, string regip, int webgold)
        {
            int num;
            DataProviders providers = new DataProviders();
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 20), new SqlParameter("@userpwd", SqlDbType.VarChar, 20), new SqlParameter("@key", SqlDbType.VarChar, 20), new SqlParameter("@sex", SqlDbType.VarChar, 50), new SqlParameter("@email", SqlDbType.VarChar, 50), new SqlParameter("@commend", SqlDbType.VarChar, 50), new SqlParameter("@regip", SqlDbType.VarChar, 50), new SqlParameter("@webgold", SqlDbType.Int, 4) };
            parameters[0].Value = userid;
            parameters[1].Value = userpwd;
            parameters[2].Value = key;
            parameters[3].Value = sex;
            parameters[4].Value = email;
            parameters[5].Value = commend;
            parameters[6].Value = regip;
            parameters[7].Value = webgold;
            switch (providers.RunProcedure("Web_Register", parameters, out num))
            {
                case 0:
                    return "恭喜您，帳號註冊成功, 需要使用網站功能請先登陸";

                case 1:
                    return "對不起，此用戶已經被註冊！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string resetpoint(string userid, int useridx, int chaidx, int resetpointmoney, int resetpointgold, int charesetgivepoint)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@useridx", SqlDbType.Int, 8), new SqlParameter("@chaidx", SqlDbType.Int, 8), new SqlParameter("@resetpointmoney", SqlDbType.Int, 8), new SqlParameter("@resetpointgold", SqlDbType.Int, 8), new SqlParameter("@charesetgivepoint", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = useridx;
            parameters[2].Value = chaidx;
            parameters[3].Value = resetpointmoney;
            parameters[4].Value = resetpointgold;
            parameters[5].Value = charesetgivepoint;
            switch (new DataProviders().RunProcedure("Web_ResetPoint", parameters, out num))
            {
                case 0:
                    return "在線洗點成功，請登陸遊戲查看。";

                case 1:
                    return "請先退出遊戲至登陸界面後再來洗點！";

                case 2:
                    return "你的金幣不夠，不能洗點！";

                case 3:
                    return "你的遊戲幣不夠，不能洗點！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string signpublicsale(string userid, int pssigngold, int psprice, int psid)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@signprice", SqlDbType.Int, 8), new SqlParameter("@psprice", SqlDbType.Int, 8), new SqlParameter("@psid", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = pssigngold;
            parameters[2].Value = psprice;
            parameters[3].Value = psid;
            switch (new DataProviders().RunProcedure("Web_SignPublicSale", parameters, out num))
            {
                case 0:
                    return "競價成功,請隨時關注拍賣情況,別人可能會超過你唷。";

                case 1:
                    return "拍賣物品不存在或者拍賣已結束！";

                case 2:
                    return "對不起,已經有人出的價格比你更高了,請返回重新確認後再競價！";

                case 3:
                    return "對不起,您的金幣數量不足,不能競拍,請返回確認！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string skilllevelup(string userid, int chaidx, int mugongidx, int needmoney)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@character_idx", SqlDbType.Int, 8), new SqlParameter("@mugong_dbidx", SqlDbType.Int, 8), new SqlParameter("@needmoney", SqlDbType.Int, 8) };
            parameters[0].Value = userid;
            parameters[1].Value = chaidx;
            parameters[2].Value = mugongidx;
            parameters[3].Value = needmoney;
            switch (new DataProviders().RunProcedure("Web_SkillLevelUP", parameters, out num))
            {
                case 0:
                    return "武功升級成功，請登陸遊戲查看。";

                case 1:
                    return "請先退出遊戲至登陸界面後再來升級武功！";

                case 2:
                    return "武功最多只能升到12級,你已經超過了！";

                case 3:
                    return "角色所帶遊戲幣不夠，不能升級武功！";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string stoppublicsale(int psid)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@psid", SqlDbType.Int, 8) };
            parameters[0].Value = psid;
            if (new DataProviders().RunProcedure("Web_StopPublicSale", parameters, out num) == 0)
            {
                return "此拍賣物品已經結束拍賣，所有金幣已經返還給玩家。";
            }
            return "系統級錯誤，請聯繫管理員！";
        }

        public string uniteguild(string userid, string userpwd, string newchaname, string olduserid, string olduserpwd, string oldchaname)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@userpwd", SqlDbType.VarChar, 50), new SqlParameter("@newchaname", SqlDbType.VarChar, 50), new SqlParameter("@olduserid", SqlDbType.VarChar, 50), new SqlParameter("@olduserpwd", SqlDbType.VarChar, 50), new SqlParameter("@oldchaname", SqlDbType.VarChar, 50) };
            parameters[0].Value = userid;
            parameters[1].Value = userpwd;
            parameters[2].Value = newchaname;
            parameters[3].Value = olduserid;
            parameters[4].Value = olduserpwd;
            parameters[5].Value = oldchaname;
            int num2 = new DataProviders().RunProcedure("Web_UniteGuild", parameters, out num);
            switch (num2)
            {
                case 0:
                    return "您的幫會已經成功合到二區";

                case 1:
                    return "新區角色不存在,請重新核對角色名";

                case 2:
                    return "舊區角色不存在,請重新核對角色名";

                case 3:
                    return "新區賬號或密碼錯誤,請重新核對";

                case 4:
                    return "舊區賬號或密碼錯誤,請重新核對";

                case 5:
                    return "此幫會已經合過了,不能再合嘍";

                case 6:
                    return "新區的角色不是幫主,不能合幫";

                case 7:
                    return "舊區的角色不是幫主,不能合幫";
            }
            return ("系統級錯誤，請聯繫管理員！錯誤代碼:" + num2.ToString());
        }

        public string uniteserver(string userid, string userpwd, string server)
        {
            int num;
            SqlParameter[] parameters = new SqlParameter[] { new SqlParameter("@userid", SqlDbType.VarChar, 50), new SqlParameter("@userpwd", SqlDbType.VarChar, 50) };
            parameters[0].Value = userid;
            parameters[1].Value = userpwd;
            int num2 = new DataProviders().RunProcedure(server, parameters, out num);
            switch (num2)
            {
                case 0:
                    return "恭喜,您已經成功合到國際新區,請登陸新區查看";

                case 1:
                    return "賬號或密碼錯誤";

                case 2:
                    return "此賬號已經合至新區了,請登陸新區查看";
            }
            return ("系統級錯誤，請聯繫管理員！錯誤代碼:" + num2.ToString());
        }
    }
}
