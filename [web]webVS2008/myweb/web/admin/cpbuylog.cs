namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpbuylog : Page
    {
        protected Button btnsearch;
        protected DataGrid DataGrid2;
        protected TextBox tbplayerid;

        private void btnsearch_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbplayerid.Text.ToString().Trim());
            this.DataGrid2.DataSource = new DataProviders().ExecuteSqlDs("select a.userid,b.name,buydate,a.gold from web_buylog a,web_item b where userid='" + str + "' and a.itemidx=b.itemid order by buydate desc", "DataGrid2");
            this.DataGrid2.DataBind();
        }

        private void InitializeComponent()
        {
            this.btnsearch.Click += new EventHandler(this.btnsearch_Click);
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
        }
    }
}
