namespace web.agent
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class log : Page
    {
        protected Button btnlogout;
        protected DataGrid DataGrid1;
        protected DataGrid DataGrid2;
        protected Button tbsearch;
        protected TextBox tbuserid;

        private void btnlogout_Click(object sender, EventArgs e)
        {
            this.Session.Abandon();
            base.Response.Redirect("default.aspx");
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbuserid.Text.ToString());
            this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where agentid='" + this.Session["agent_id"].ToString() + "' and playerid='" + str + "' and type='代理發放金幣' order by date desc", "DataGrid2");
            this.DataGrid2.DataBind();
        }

        private void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where agentid='" + this.Session["agent_id"].ToString() + "' and type='管理員發放金幣' order by date desc", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid2_Delete(object sender, DataGridCommandEventArgs e)
        {
            int id = int.Parse(e.Item.Cells[0].Text);
            string str = new WebLogic().getbackgold(id);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void DataGrid2_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid2.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where agentid='" + this.Session["agent_id"].ToString() + "' and type='代理發放金幣' order by date desc", "DataGrid2");
            this.DataGrid2.DataBind();
        }

        private void InitializeComponent()
        {
            this.btnlogout.Click += new EventHandler(this.btnlogout_Click);
            this.DataGrid1.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid1_PageIndexChanged);
            this.tbsearch.Click += new EventHandler(this.Button1_Click);
            this.DataGrid2.PageIndexChanged += new DataGridPageChangedEventHandler(this.DataGrid2_PageIndexChanged);
            this.DataGrid2.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid2_Delete);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isagent();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where agentid='" + this.Session["agent_id"].ToString() + "' and type='管理員發放金幣' order by date desc", "DataGrid1");
                this.DataGrid1.DataBind();
                this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select * from mhcmember..web_log where agentid='" + this.Session["agent_id"].ToString() + "' and type='代理發放金幣' order by date desc", "DataGrid2");
                this.DataGrid2.DataBind();
            }
        }
    }
}
