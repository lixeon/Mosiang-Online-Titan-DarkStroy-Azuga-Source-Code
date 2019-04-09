namespace web.agent
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class searchuserid : Page
    {
        protected Button btnlogout;
        protected DataGrid DataGrid2;
        protected Button tbsearch;
        protected TextBox tbuserid;

        private void InitializeComponent()
        {
            this.tbsearch.Click += new EventHandler(this.tbsearch_Click);
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
        }

        private void tbsearch_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbuserid.Text.ToString());
            this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select id_loginid as user_id,id_regdate as create_time,* from mhcmember..chr_log_info where id_loginid = '" + str + "'", "DataGrid2");
            this.DataGrid2.DataBind();
        }
    }
}
