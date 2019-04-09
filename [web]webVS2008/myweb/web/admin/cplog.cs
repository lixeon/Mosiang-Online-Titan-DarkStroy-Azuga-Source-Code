namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cplog : Page
    {
        protected DataGrid DataGrid1;

        private void DataGrid1_PageIndexChanged(object sender, DataGridPageChangedEventArgs e)
        {
            this.DataGrid1.CurrentPageIndex = e.NewPageIndex;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select top 100 * from web_log where type='後台登陸日誌' order by date desc", "DataGrid1");
            this.DataGrid1.DataBind();
        }

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
            new WebLogic().diskf();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select top 100 * from web_log where type='後台登陸日誌' order by date desc", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}
