namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpincome : Page
    {
        protected DataGrid DataGrid1;
        protected DataGrid DataGrid2;
        protected DataGrid DataGrid3;

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
            new WebLogic().isadmin();
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from (select top 5 playerid,convert(varchar(10),date,120) as date,sum(gold) as gold  from mhcmember..web_log where  convert(varchar(10),date,120)=convert(varchar(10),getdate(),120) and type='代理發放金幣' group by playerid,convert(varchar(10),date,120) order by sum(gold) desc union select top 5 playerid,convert(varchar(10),date,120) as date,sum(gold) as gold  from mhcmember..web_log where  convert(varchar(10),date,120)=convert(varchar(10),getdate()-1,120) and type='代理發放金幣' group by playerid,convert(varchar(10),date,120) order by sum(gold) desc union select top 5 playerid,convert(varchar(10),date,120) as date,sum(gold) as gold  from mhcmember..web_log where  convert(varchar(10),date,120)=convert(varchar(10),getdate()-2,120) and type='代理發放金幣' group by playerid,convert(varchar(10),date,120) order by sum(gold) desc union select top 5 userid as playerid,convert(varchar(10),date,120) as date,sum(money)*10 as gold  from mhcmember..web_alipay where  convert(varchar(10),date,120)=convert(varchar(10),getdate(),120) and state=1 group by userid,convert(varchar(10),date,120) order by sum(money) desc union select top 5 userid as playerid,convert(varchar(10),date,120) as date,sum(money)*10 as gold  from mhcmember..web_alipay where  convert(varchar(10),date,120)=convert(varchar(10),getdate()-1,120) and state=1 group by userid,convert(varchar(10),date,120) order by sum(money) desc union select top 5 userid as playerid,convert(varchar(10),date,120) as date,sum(money) as gold  from mhcmember..web_alipay where  convert(varchar(10),date,120)=convert(varchar(10),getdate()-2,120) and state=1 group by userid,convert(varchar(10),date,120) order by sum(money) desc) b order by date desc,gold desc", "DataGrid1");
            this.DataGrid1.DataBind();
            this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select * from (select top 5 playerid,'第一周' as date,sum(gold) as gold  from mhcmember..web_log where  convert(varchar(10),date,120) between '2008-01-01' and '2008-01-07' and type='代理發放金幣' group by playerid,date order by sum(gold) desc union select top 5 playerid,'第二周' as date,sum(gold) as gold  from mhcmember..web_log where convert(varchar(10),date,120) between '2008-01-08' and '2008-01-14' and type='代理發放金幣' group by playerid,date order by sum(gold) desc union select top 5 playerid,'第三周' as date,sum(gold) as gold  from mhcmember..web_log where  convert(varchar(10),date,120) between '2008-01-15' and '2008-01-22' and type='代理發放金幣' group by playerid,date order by sum(gold) desc union select top 5 playerid,'第四周' as date,sum(gold) as gold  from mhcmember..web_log where  convert(varchar(10),date,120) between '2008-01-23' and '2008-01-31' and type='代理發放金幣' group by playerid,date order by sum(gold) desc union select top 5 userid as playerid,'第一周' as date,sum(money)*10 as gold  from mhcmember..web_alipay where  convert(varchar(10),date,120) between '2008-01-1' and '2008-01-7' and state=1 group by userid,date order by gold desc union  select top 5 userid as playerid,'第二周' as date,sum(money)*10 as gold  from mhcmember..web_alipay where  convert(varchar(10),date,120) between '2008-01-8' and '2008-01-14' and state=1 group by userid,date order by gold desc  union select top 5 userid as playerid,'第三周' as date,sum(money)*10 as gold  from mhcmember..web_alipay where  convert(varchar(10),date,120) between '2008-01-15' and '2008-01-22' and state=1 group by userid,date order by gold desc union select top 5 userid as playerid,'第四周' as date,sum(money)*10 as gold  from mhcmember..web_alipay where  convert(varchar(10),date,120) between '2008-01-23' and '2008-01-31' and state=1 group by userid,date order by gold desc) b order by date desc,gold desc", "DataGrid2");
            this.DataGrid2.DataBind();
            this.DataGrid3.DataSource = new DataProviders().ExecuteSqlDs("select * from (select top 5 playerid,convert(varchar(7),date,120) as date,sum(gold) as gold  from mhcmember..web_log where  convert(varchar(7),date,120)=convert(varchar(7),getdate(),120) and type='代理發放金幣' group by playerid,convert(varchar(7),date,120) order by sum(gold) desc union  select top 5 userid as playerid,convert(varchar(7),date,120) as date,sum(money)*10 as gold  from mhcmember..web_alipay where  convert(varchar(7),date,120)=convert(varchar(7),getdate(),120) and state=1 group by userid,convert(varchar(7),date,120) order by gold desc ) b order by date desc,gold desc", "DataGrid3");
            this.DataGrid3.DataBind();
        }
    }
}
