namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class download : UserControl
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
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_download order by date desc", "DataGrid1");
            this.DataGrid1.DataBind();
        }
    }
}
