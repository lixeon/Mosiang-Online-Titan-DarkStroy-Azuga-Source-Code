namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpagentlog : Page
    {
        protected Button btnsearch;
        protected DataGrid DataGrid1;
        protected DataGrid DataGrid2;
        protected DataGrid DataGrid3;
        protected DataGrid DataGrid4;
        protected TextBox tbplayerid;

        private void btnsearch_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbplayerid.Text.ToString().Trim());
            this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where type='代理發放金幣' and playerid= '" + str + "' order by date desc", "DataGrid2");
            this.DataGrid2.DataBind();
        }

        private void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where type='管理員發放金幣' order by date desc", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid2_Delete(object sender, DataGridCommandEventArgs e)
        {
            int num = int.Parse(e.Item.Cells[0].Text);
            new DataProviders().ExecuteSql("delete from web_log where id=" + num);
        }

        private void DataGrid2_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid2.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where type='代理發放金幣' order by date desc", "DataGrid2");
            this.DataGrid2.DataBind();
        }

        private void DataGrid3_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid3.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid3.DataSource = new DataProviders().ExecuteSqlDs("select convert(char(10),date,120) as date,sum(gold) as gold from web_log where type='代理發放金幣' group by convert(char(10),date,120) order by date desc", "DataGrid3");
            this.DataGrid3.DataBind();
        }

        private void DataGrid4_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid4.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid4.DataSource = new DataProviders().ExecuteSqlDs("select convert(char(7),date,120) as date,sum(gold) as gold ,agentid from web_log where type='代理發放金幣' group by convert(char(7),date,120) ,agentid order by date desc", "DataGrid4");
            this.DataGrid4.DataBind();
        }

        private void InitializeComponent()
        {
            this.DataGrid1.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid1_PageIndexChanged);
            this.btnsearch.Click += new EventHandler(this.btnsearch_Click);
            this.DataGrid2.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid2_PageIndexChanged);
            this.DataGrid2.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid2_Delete);
            this.DataGrid3.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid3_PageIndexChanged);
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
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where type='管理員發放金幣' and date<=getdate() order by date desc", "DataGrid1");
                this.DataGrid1.DataBind();
                this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where type='代理發放金幣' order by date desc", "DataGrid2");
                this.DataGrid2.DataBind();
                this.DataGrid3.DataSource = new DataProviders().ExecuteSqlDs("select convert(char(10),date,120) as date,sum(gold) as gold from web_log where type='代理發放金幣' group by convert(char(10),date,120) order by date desc", "DataGrid3");
                this.DataGrid3.DataBind();
                this.DataGrid4.DataSource = new DataProviders().ExecuteSqlDs("select convert(char(7),date,120) as date,sum(gold) as gold ,agentid from web_log where type='代理發放金幣' group by convert(char(7),date,120) ,agentid order by date desc", "DataGrid4");
                this.DataGrid4.DataBind();
            }
        }
    }
}
