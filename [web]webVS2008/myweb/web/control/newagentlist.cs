namespace web.control
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using web;

    public class newagentlist : UserControl
    {
        public string gold;
        public string str;

        private void InitializeComponent()
        {
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            this.gold = "0";
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select top 1 playerid as gold from (select top 1 playerid,convert(varchar(10),date,120) as date,sum(gold) as gold  from mhcmember..web_log where  convert(varchar(10),date,120)=convert(varchar(10),getdate(),120) and type='代理發放金幣' group by playerid,convert(varchar(10),date,120) order by sum(gold) desc union select top 1 userid as playerid,convert(varchar(10),date,120) as date,sum(money)*10 as gold from web_alipay where convert(varchar(10),date,120)=convert(varchar(10),getdate(),120) and state=1 group by userid,convert(varchar(10),date,120) order by gold desc) b order by gold desc");
            if (reader.Read())
            {
                this.gold = reader["gold"].ToString();
            }
            if (this.gold != "0")
            {
                string str = this.gold.Substring(0, 1).ToString();
                string str2 = this.gold.Substring(3).ToString();
                this.gold = str + "**" + str2;
            }
            reader.Close();
            reader = providers.ExecuteSqlDataReader("SELECT [id],[name], [nation],  '充足' as gold FROM [MHCMEMBER].[dbo].[Web_Agent] where  [state]=1");
            while (reader.Read())
            {
                this.str = this.str + "<tr height=\"20\"><td width=\"10\"></td>";
                string str3 = this.str;
                this.str = str3 + "<td width=\"241\" align=center height=\"20\"><a href=viewagent.aspx?id=" + reader["id"].ToString() + ">" + reader["name"].ToString() + "</a></td>";
                this.str = this.str + "<td width=\"12\"></td></tr>";
            }
            providers.CloseConn();
        }
    }
}
