namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class agentlist : UserControl
    {
        protected DataGrid DataGrid1;

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
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from (select top 1 0 as id,'今日贊助' as name,'最高金幣' as nation ,convert(varchar,gold) as gold from (select top 1 playerid,convert(varchar(10),date,120) as date,sum(gold) as gold  from mhcmember..web_log where  convert(varchar(10),date,120)=convert(varchar(10),getdate(),120) and type='代理發放金幣' group by playerid,convert(varchar(10),date,120) order by sum(gold) desc union select top 1 userid as playerid,convert(varchar(10),date,120) as date,sum(money)*10 as gold from web_alipay where convert(varchar(10),date,120)=convert(varchar(10),getdate(),120) and state=1 group by userid,convert(varchar(10),date,120) order by gold desc) b order by gold desc union SELECT  [id],[name], [nation],  '充足' as gold FROM [MHCMEMBER].[dbo].[Web_Agent] where  [state]=1) x", "DataGrid1");
            this.DataGrid1.DataBind();
        }
    }
}
